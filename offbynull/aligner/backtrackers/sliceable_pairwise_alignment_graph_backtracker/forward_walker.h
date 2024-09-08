#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_H

#include <cstddef>
#include <functional>
#include <ranges>
#include <optional>
#include <utility>
#include <stdexcept>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/slot.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container_pair.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::concepts::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::concepts::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::slot::slot;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container::row_slot_container;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container_pair
        ::row_slot_container_pair;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_with_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_stack_container_creator_pack;
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::range_of_type;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::concepts::unqualified_value_type;




    template<
        typename T,
        typename N,
        typename E,
        typename ED
    >
    concept forward_walker_container_creator_pack =
        unqualified_value_type<T>
        && backtrackable_node<N>
        && backtrackable_edge<E>
        && weight<ED>
        && requires(const T t) {
            { t.create_row_slot_container_container_creator_pack() } -> row_slot_container_container_creator_pack<E, ED>;
            { t.create_resident_slot_container_container_creator_pack() } -> resident_slot_container_container_creator_pack<N, E, ED>;
        };

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        bool minimize_allocations
    >
    struct forward_walker_heap_container_creator_pack {
        row_slot_container_heap_container_creator_pack<
            debug_mode,
            E,
            ED
        > create_row_slot_container_container_creator_pack() const {
            return {};
        }

        resident_slot_container_heap_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            minimize_allocations
        > create_resident_slot_container_container_creator_pack() const {
            return {};
        }
    };

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        std::size_t grid_right_cnt,
        std::size_t grid_depth_cnt,
        std::size_t resident_nodes_capacity
    >
    struct forward_walker_stack_container_creator_pack {
        row_slot_container_stack_container_creator_pack<
            debug_mode,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt
        > create_row_slot_container_container_creator_pack() const {
            return {};
        }

        resident_slot_container_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            resident_nodes_capacity
        > create_resident_slot_container_container_creator_pack() const {
            return {};
        }
    };




    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct row_entry {
        N node;
        slot<E, WEIGHT>* slot_ptr;

        row_entry()
        : node {}
        , slot_ptr { nullptr } {}
    };

    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G,
        forward_walker_container_creator_pack<
            typename G::N,
            typename G::E,
            typename G::ED
        > CONTAINER_CREATOR_PACK = forward_walker_heap_container_creator_pack<
            debug_mode,
            typename G::N,
            typename G::E,
            typename G::ED,
            true
        >
    >
    class forward_walker {
    private:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        using ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK =
            decltype(std::declval<CONTAINER_CREATOR_PACK>().create_row_slot_container_container_creator_pack());
        using RESIDENT_SLOT_CONTAINER_CONTAINER_CREATOR_PACK =
            decltype(std::declval<CONTAINER_CREATOR_PACK>().create_resident_slot_container_container_creator_pack());

        const G& g;
        resident_slot_container<debug_mode, G, RESIDENT_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> resident_slots;
        row_slot_container_pair<debug_mode, G, ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> row_slots;
        decltype(g.row_nodes(0u)) row;
        decltype(row.begin()) row_it;
        row_entry<N, E, ED> row_entry_;

    public:
        static forward_walker create_and_initialize(
            const G& g_,
            const INDEX target_row,
            CONTAINER_CREATOR_PACK container_creator_pack_ = {}
        ) {
            if constexpr (debug_mode) {
                if (target_row >= g_.grid_down_cnt) {
                    throw std::runtime_error { "Slice too far down" };
                }
            }
            forward_walker ret {
                g_,
                container_creator_pack_
            };
            while (ret.row_slots.grid_down_offset != target_row || ret.row_it != ret.row.end()) {
                ret.step_forward();
            }
            return ret;
        }

        slot<E, ED>& find(const N& node) {
            auto found_resident { resident_slots.find(node) };
            if (found_resident.has_value()) {
                return (*found_resident).get().slot_;
            }
            auto found_row { row_slots.find(node) };
            if (found_row.has_value()) {
                return *found_row;
            }
            if constexpr (debug_mode) {
                throw std::runtime_error { "Node not found" };
            }
            std::unreachable();
        }

    private:
        forward_walker(
            const G& g_,
            CONTAINER_CREATOR_PACK container_creator_pack_ = {}
        )
        : g { g_ }
        , resident_slots { g, container_creator_pack_.create_resident_slot_container_container_creator_pack() }
        , row_slots { g, container_creator_pack_.create_row_slot_container_container_creator_pack() }
        , row { g.row_nodes(0u) }
        , row_it { row.begin() }
        , row_entry_ {} {
            row_entry_.node = *row_it;
            row_entry_.slot_ptr = &find(row_entry_.node); // should be equivalent to g.get_root_node()
        }

        void step_forward() {
            if (row_it != row.end()) {
                row_entry_.node = *row_it;
                row_entry_.slot_ptr = &find(row_entry_.node);
            } else {
                if (row_slots.grid_down_offset == g.grid_down_cnt - 1u) {
                    return;
                }
                row_slots.move_down();
                row = g.row_nodes(row_slots.grid_down_offset);
                row_it = row.begin();
                row_entry_.node = *row_it;
                row_entry_.slot_ptr = &find(row_entry_.node);
            }
            // Compute only if node is not a resident. A resident node's backtracking weight + backtracking edge should
            // be computed as its inputs are walked over one-by-one by this function (see block below this one).
            if (!resident_slots.find(row_entry_.node).has_value()) {
                auto incoming_accumulated {
                    std::views::common(
                        g.get_inputs(row_entry_.node)
                        | std::views::transform(
                            [&](const auto& edge) -> std::pair<E, ED> {
                                const N& n_from { g.get_edge_from(edge) };
                                const ED& edge_weight { g.get_edge_data(edge) };
                                slot<E, ED>& n_from_slot { find(n_from) };
                                return { edge, n_from_slot.backtracking_weight + edge_weight };
                            }
                        )
                    )
                };
                auto found {
                    std::ranges::max_element(
                        incoming_accumulated.begin(),
                        incoming_accumulated.end(),
                        [](const std::pair<E, ED>& a, const std::pair<E, ED>& b) {
                            return a.second < b.second;
                        }
                    )
                };
                if (found != incoming_accumulated.end()) {  // if no incoming nodes found, it's a root node
                    row_entry_.slot_ptr->backtracking_edge = { (*found).first };
                    row_entry_.slot_ptr->backtracking_weight = (*found).second;
                }
            }

            // Update resident node weights
            for (const E& edge : g.outputs_to_residents(row_entry_.node)) {
                const N& resident_node { g.get_edge_to(edge) };
                std::optional<std::reference_wrapper<resident_slot<E, ED>>> resident_slot_maybe {
                    resident_slots.find(resident_node)
                };
                if constexpr (debug_mode) {
                    if (!resident_slot_maybe.has_value()) {
                        throw std::runtime_error { "This should never happen" };
                    }
                }
                resident_slot<E, ED>& resident_slot_ { (*resident_slot_maybe).get() };
                const ED& edge_weight { g.get_edge_data(edge) };
                if (!resident_slot_.initialized) {
                    resident_slot_.slot_.backtracking_edge = { edge };
                    resident_slot_.slot_.backtracking_weight = edge_weight;
                    resident_slot_.initialized = true;
                } else {
                    const ED& new_weight { row_entry_.slot_ptr->backtracking_weight + edge_weight };
                    if (new_weight > resident_slot_.slot_.backtracking_weight) {
                        resident_slot_.slot_.backtracking_edge = { edge };
                        resident_slot_.slot_.backtracking_weight = new_weight;
                    }
                }
            }

            // Move to next node
            ++row_it;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_H
