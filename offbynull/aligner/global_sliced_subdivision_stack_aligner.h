#ifndef OFFBYNULL_ALIGNER_GLOBAL_SLICED_SUBDIVISION_STACK_ALIGNER_H
#define OFFBYNULL_ALIGNER_GLOBAL_SLICED_SUBDIVISION_STACK_ALIGNER_H

#include <utility>
#include <type_traits>
#include <concepts>
#include <cstddef>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <ranges>
#include "offbynull/aligner/aligner_utils.h"
#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtracker.h"
#include "offbynull/concepts.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::global_sliced_subdivision_stack_aligner {
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::concepts::numeric;
    using offbynull::utils::wider_numeric;
    using offbynull::utils::compile_time_constant;
    using offbynull::aligner::aligner_utils::find_narrowest_index_type;
    using offbynull::aligner::aligner_utils::index_marker_onehot;
    using offbynull::aligner::aligner_utils::marker_to_type;
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::scorer::scorer::scorer;
    using offbynull::aligner::scorer::scorer::scorer_without_explicit_weight_or_seq_index;
    using offbynull::aligner::scorers::widening_scorer::widening_scorer;
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::graphs::pairwise_global_alignment_graph::pairwise_global_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtracker_stack_container_creator_pack
        ::backtracker_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtracker::backtracker;


    /**
     * Concept that's satisfied if `T` is has the traits to configure a sliced subdivision global alignment to use stack-based memory.
     *
     * @tparam T Type to check.
     */
    template<typename T>
    concept sliced_subdivision_stack_allocation_strategy = requires {
        { T::max_down_seq_size } -> std::same_as<const std::size_t &>;  // Maximum down sequence size
        compile_time_constant<T::max_down_seq_size> {};
        { T::max_right_seq_size } -> std::same_as<const std::size_t &>;  // Maximum right sequence size
        compile_time_constant<T::max_right_seq_size> {};
    };

    /**
     * Standard @ref sliced_subdivision_stack_allocation_strategy implementation.
     *
     * @tparam max_down_seq_size_ Maximum allowed length for downward sequence.
     * @tparam max_right_seq_size_ Maximum allowed length for rightward sequence.
     */
    template<std::size_t max_down_seq_size_, std::size_t max_right_seq_size_>
    struct sliced_subdivision_stack_allocation_strategy_config {
        /** Maximum allowed length for downward sequence (copy of `max_down_seq_size_`). */
        static constexpr std::size_t max_down_seq_size { max_down_seq_size_ };
        /** Maximum allowed length for rightward sequence (copy of `max_right_seq_size_`). */
        static constexpr std::size_t max_right_seq_size { max_right_seq_size_ };
    };

    template<bool debug_mode>
    class global_sliced_subdivision_stack_aligner {
    public:
        template<
            sliced_subdivision_stack_allocation_strategy allocation_strategy,
            widenable_to_size_t INDEX = std::size_t
        >
        auto align(
            const sequence auto& down,
            const sequence auto& right,
            const scorer_without_explicit_weight_or_seq_index<
                std::remove_cvref_t<decltype(down[0])>,
                std::remove_cvref_t<decltype(right[0])>
            > auto& substitution_scorer,
            const scorer_without_explicit_weight_or_seq_index<
                std::remove_cvref_t<decltype(down[0])>,
                std::remove_cvref_t<decltype(right[0])>
            > auto& gap_scorer,
            const weight auto max_path_weight_comparison_tolerance
        ) {
            // Check for rollover on +1
            if constexpr (debug_mode) {
                INDEX down_node_cnt { static_cast<INDEX>(down.size() + 1zu) };
                INDEX right_node_cnt { static_cast<INDEX>(right.size() + 1zu) };
                if (down_node_cnt != down.size() + 1zu || right_node_cnt != right.size() + 1zu) {
                    throw std::runtime_error { "Index type too narrow" };
                }
            }
            // Check INDEX to ensure it can hold sequences
            using SUBSTITUTION_SCORER = std::remove_cvref_t<decltype(substitution_scorer)>;
            using GAP_SCORER = std::remove_cvref_t<decltype(gap_scorer)>;
            if constexpr (debug_mode) {
                if (std::max(down.size(), right.size()) == std::numeric_limits<std::size_t>::max()) {
                    throw std::runtime_error { "Sequence too large" };  // Needed because of +1 below - no reasonable way this happens?
                }
                std::size_t index_max { std::max(down.size(), right.size()) + 1zu };
                if (index_max > std::numeric_limits<typename SUBSTITUTION_SCORER::SEQ_INDEX>::max()
                        || index_max > std::numeric_limits<typename GAP_SCORER::SEQ_INDEX>::max()) {
                    throw std::runtime_error { "Scorer's sequence index type too narrow to support a sequence this large" };
                }
            }

            // Derive weight type by getting the wider type between scorers.
            // NOTE: Technically, weight can be non-numeric (it just needs to have certain type traits), but in this case it must be
            //       numeric to determine which one is wider.
            using SUBSTITUTION_WEIGHT = typename SUBSTITUTION_SCORER::WEIGHT;
            using GAP_WEIGHT = typename GAP_SCORER::WEIGHT;
            static_assert(numeric<SUBSTITUTION_WEIGHT>, "weight must be numeric");
            static_assert(numeric<GAP_WEIGHT>, "weight must be numeric");
            using WEIGHT = typename wider_numeric<SUBSTITUTION_WEIGHT, GAP_WEIGHT>::type;

            // Check max_path_weight_comparison_tolerance doesn't narrow
            using WEIGHT_TOLERANCE = decltype(max_path_weight_comparison_tolerance);
            static_assert(numeric<WEIGHT_TOLERANCE>, "weight must be numeric");
            if constexpr (debug_mode) {
                if (max_path_weight_comparison_tolerance != static_cast<WEIGHT>(max_path_weight_comparison_tolerance)) {
                    throw std::runtime_error { "Max path weight tolerance not representable by WEIGHT type" };
                }
            }

            // Wrap scorers into ones that'll cast from INDEX to the scorer's SEQ_INDEX (widens, doesn't narrow). Likewise, the
            // wrapped scorer's result will be cast to WEIGHT type derived above (widens). This is to make it easier for the user
            // (e.g., they submit scorers with mismatching INDEX / WEIGHT).
            using DOWN_ELEM = std::remove_cvref_t<decltype(down[0])>;
            using RIGHT_ELEM = std::remove_cvref_t<decltype(right[0])>;
            const widening_scorer<
                debug_mode,
                INDEX,
                DOWN_ELEM,
                RIGHT_ELEM,
                WEIGHT,
                std::remove_cvref_t<decltype(substitution_scorer)>
            > widened_substitution_scorer {
                substitution_scorer
            };
            const widening_scorer<
                debug_mode,
                INDEX,
                DOWN_ELEM,
                RIGHT_ELEM,
                WEIGHT,
                std::remove_cvref_t<decltype(gap_scorer)>
            > widened_gap_scorer {
                gap_scorer
            };

            return align_strict<INDEX, WEIGHT, allocation_strategy>(
                down,
                right,
                widened_substitution_scorer,
                widened_gap_scorer,
                max_path_weight_comparison_tolerance
            );
        }

        template<
            widenable_to_size_t INDEX,
            weight WEIGHT,
            sliced_subdivision_stack_allocation_strategy allocation_strategy
        >
        auto align_strict(
            const sequence auto& down,
            const sequence auto& right,
            const scorer<
                INDEX,
                std::remove_cvref_t<decltype(down[0])>,
                std::remove_cvref_t<decltype(right[0])>,
                WEIGHT
            > auto& substitution_scorer,
            const scorer<
                INDEX,
                std::remove_cvref_t<decltype(down[0])>,
                std::remove_cvref_t<decltype(right[0])>,
                WEIGHT
            > auto& gap_scorer,
            const WEIGHT max_path_weight_comparison_tolerance
        ) {
            if constexpr (debug_mode) {
                if (down.size() > allocation_strategy::max_down_seq_size
                        || right.size() > allocation_strategy::max_right_seq_size) {
                    throw std::runtime_error { "Sequence length larger than expected (for stack allocation)" };
                }
            }
            using G = pairwise_global_alignment_graph<
                debug_mode,
                INDEX,
                WEIGHT,
                std::remove_cvref_t<decltype(down)>,
                std::remove_cvref_t<decltype(right)>,
                std::remove_cvref_t<decltype(substitution_scorer)>,
                std::remove_cvref_t<decltype(gap_scorer)>
            >;
            G graph {
                down,
                right,
                substitution_scorer,
                gap_scorer
            };
            ;
            using CONTAINER_CREATOR_PACK = backtracker_stack_container_creator_pack<
                debug_mode,
                typename G::N,
                typename G::E,
                typename G::ED,
                allocation_strategy::max_right_seq_size + 1zu,
                1zu,  // 1 depth
                0zu,  // no resident nodes
                allocation_strategy::max_down_seq_size + allocation_strategy::max_right_seq_size
            >;
            backtracker<
                debug_mode,
                G,
                CONTAINER_CREATOR_PACK
            > backtracker_ {};
            auto&& [path, path_weight] {  // Use auto&& vs const auto&, otherwise move() below fails
                backtracker_.find_max_path(graph, max_path_weight_comparison_tolerance)
            };
            return std::make_pair(
                std::move(path)  // move() needed to ensure that transform operation below owns path
                    | std::views::transform(
                        [graph](const typename G::E& edge) {  // graph lambda capture must be copy/move to avoid dangling ref
                            return graph.edge_to_element_offsets(edge);
                        }
                    ),
                path_weight
            );
        }
    };

}

#endif //OFFBYNULL_ALIGNER_GLOBAL_SLICED_SUBDIVISION_STACK_ALIGNER_H
