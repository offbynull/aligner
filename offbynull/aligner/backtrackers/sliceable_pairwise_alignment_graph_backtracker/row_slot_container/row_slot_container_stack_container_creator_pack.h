#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_ROW_SLOT_CONTAINER_ROW_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_ROW_SLOT_CONTAINER_ROW_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/utils.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/slot.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container::slot::slot;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::concepts::weight;
    using offbynull::utils::static_vector_typer;

    template<
        bool debug_mode,
        backtrackable_edge E,
        weight ED,
        std::size_t grid_right_cnt,
        std::size_t grid_depth_cnt
    >
    struct row_slot_container_stack_container_creator_pack {
        static constexpr std::size_t max_elem_cnt { grid_right_cnt * grid_depth_cnt };
        using SEGMENT_CONTAINER_TYPE = typename static_vector_typer<
            debug_mode,
            slot<E, ED>,
            max_elem_cnt
        >::type;
        SEGMENT_CONTAINER_TYPE create_slot_container(std::size_t grid_right_cnt_, std::size_t grid_depth_cnt_) const {
            std::size_t cnt { grid_right_cnt_ * grid_depth_cnt_ };
            if constexpr (debug_mode) {
                if (cnt > max_elem_cnt) {
                    throw std::runtime_error { "Bad element count" };
                }
            }
            return SEGMENT_CONTAINER_TYPE(cnt);
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_ROW_SLOT_CONTAINER_ROW_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
