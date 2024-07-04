#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_TEST_UTILS_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_TEST_UTILS_H

#include <stdfloat>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"
#include "offbynull/aligner/graphs/pairwise_local_alignment_graph.h"
#include "offbynull/aligner/graphs/pairwise_fitting_alignment_graph.h"
#include "offbynull/aligner/graphs/pairwise_extended_gap_alignment_graph.h"
#include "offbynull/aligner/graphs/directed_graph.h"

namespace offbynull::aligner::backtrackers::test_utils {
    using offbynull::aligner::graphs::pairwise_global_alignment_graph::pairwise_global_alignment_graph;
    using offbynull::aligner::graphs::pairwise_local_alignment_graph::pairwise_local_alignment_graph;
    using offbynull::aligner::graphs::pairwise_fitting_alignment_graph::pairwise_fitting_alignment_graph;
    using offbynull::aligner::graphs::pairwise_extended_gap_alignment_graph::pairwise_extended_gap_alignment_graph;
    using offbynull::aligner::concepts::weight;

    auto create_static_match_lookup_functor(
        std::float64_t match_score,
        std::float64_t mismatch_score
    ) {
        return
            [=](
                const auto& edge,
                const auto& down_elem,
                const auto& right_elem
            ) {
                if (down_elem == right_elem) {
                    return match_score;
                } else {
                    return mismatch_score;
                }
            };
    }

    template <typename T, typename DOWN_ELEM_TYPE, typename RIGHT_ELEM_TYPE>
    concept match_lookup = requires(
        T t,
        std::pair<
            std::reference_wrapper<const DOWN_ELEM_TYPE>,
            std::reference_wrapper<const RIGHT_ELEM_TYPE>
        > elem_pair
    ) {
        { t[elem_pair] } -> weight;
    };

    template <typename DOWN_ELEM_TYPE, typename RIGHT_ELEM_TYPE>
    auto create_matrix_match_lookup_functor(
        const match_lookup<DOWN_ELEM_TYPE, RIGHT_ELEM_TYPE> auto& lookup_table
    ) {
        return
            [&](
                const auto& edge,
                const auto& down_elem,
                const auto& right_elem
            ) {
                std::reference_wrapper<const DOWN_ELEM_TYPE> down_elem_ref { down_elem };
                std::reference_wrapper<const RIGHT_ELEM_TYPE> right_elem_ref { right_elem };
                std::pair<
                    std::reference_wrapper<const DOWN_ELEM_TYPE>,
                    std::reference_wrapper<const RIGHT_ELEM_TYPE>
                > elem_pair { { down_elem }, { right_elem } };
                return lookup_table[elem_pair];
            };
    }

    auto create_static_indel_lookup_functor(
        std::float64_t indel_score
    ) {
        return
            [=](
                const auto& edge
            ) -> std::float64_t {
                return indel_score;
            };
    }

    auto create_static_freeride_lookup_functor(
        std::float64_t freeride_score
    ) {
        return
            [=](
                const auto& edge
            ) -> std::float64_t {
                return freeride_score;
            };
    }

    auto create_static_extended_indel_lookup_functor(
        std::float64_t extended_indel_score
    ) {
        return
            [=](
                const auto& edge
            ) -> std::float64_t {
                return extended_indel_score;
        };
    }

    auto create_global_alignment_graph(
        const auto& down_seq,
        const auto& right_seq,
        std::float64_t match_score,
        std::float64_t mismatch_score,
        std::float64_t indel_score
    ) {
        return
            pairwise_global_alignment_graph<
                std::decay_t<decltype(down_seq)>,
                std::decay_t<decltype(right_seq)>
            >(
                down_seq,
                right_seq,
                create_static_match_lookup_functor(match_score, mismatch_score),
                create_static_indel_lookup_functor(indel_score)
            );
    }

    auto create_local_alignment_graph(
        const auto& down_seq,
        const auto& right_seq,
        std::float64_t match_score,
        std::float64_t mismatch_score,
        std::float64_t indel_score,
        std::float64_t freeride_score
    ) {
        return
            pairwise_local_alignment_graph<
                std::decay_t<decltype(down_seq)>,
                std::decay_t<decltype(right_seq)>
            >(
                down_seq,
                right_seq,
                create_static_match_lookup_functor(match_score, mismatch_score),
                create_static_indel_lookup_functor(indel_score),
                create_static_freeride_lookup_functor(freeride_score)
            );
    }

    auto create_fitting_alignment_graph(
        const auto& down_seq,
        const auto& right_seq,
        std::float64_t match_score,
        std::float64_t mismatch_score,
        std::float64_t indel_score,
        std::float64_t freeride_score
    ) {
        return
            pairwise_fitting_alignment_graph<
                std::decay_t<decltype(down_seq)>,
                std::decay_t<decltype(right_seq)>
            >(
                down_seq,
                right_seq,
                create_static_match_lookup_functor(match_score, mismatch_score),
                create_static_indel_lookup_functor(indel_score),
                create_static_freeride_lookup_functor(freeride_score)
            );
    }

    auto create_overlap_alignment_graph(
        const auto& down_seq,
        const auto& right_seq,
        std::float64_t match_score,
        std::float64_t mismatch_score,
        std::float64_t indel_score,
        std::float64_t freeride_score
    ) {
        return
            pairwise_fitting_alignment_graph<
                std::decay_t<decltype(down_seq)>,
                std::decay_t<decltype(right_seq)>
            >(
                down_seq,
                right_seq,
                create_static_match_lookup_functor(match_score, mismatch_score),
                create_static_indel_lookup_functor(indel_score),
                create_static_freeride_lookup_functor(freeride_score)
            );
    }

    auto create_extended_gap_alignment_graph(
        const auto& down_seq,
        const auto& right_seq,
        std::float64_t match_score,
        std::float64_t mismatch_score,
        std::float64_t indel_score,
        std::float64_t extended_indel_score,
        std::float64_t freeride_score
    ) {
        return
            pairwise_extended_gap_alignment_graph<
                std::decay_t<decltype(down_seq)>,
                std::decay_t<decltype(right_seq)>
            >(
                down_seq,
                right_seq,
                create_static_match_lookup_functor(match_score, mismatch_score),
                create_static_indel_lookup_functor(indel_score),
                create_static_freeride_lookup_functor(freeride_score)
            );
    }
}


#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_TEST_UTILS_H
