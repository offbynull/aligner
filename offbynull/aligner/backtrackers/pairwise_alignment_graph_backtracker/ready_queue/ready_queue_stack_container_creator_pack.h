#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack {
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::utils::static_vector_typer;

    template<
        bool debug_mode,
        widenable_to_size_t SLOT_INDEX,
        std::size_t grid_down_cnt,
        std::size_t grid_right_cnt,
        std::size_t grid_depth_cnt
    >
    struct ready_queue_stack_container_creator_pack {
        static constexpr std::size_t ELEM_COUNT { grid_down_cnt * grid_right_cnt * grid_depth_cnt };
        using CONTAINER_TYPE = typename static_vector_typer<debug_mode, SLOT_INDEX, ELEM_COUNT>::type;
        CONTAINER_TYPE create_queue_container(
            std::size_t grid_down_cnt_,
            std::size_t grid_right_cnt_,
            std::size_t grid_depth_cnt_
        ) const {
            if constexpr (debug_mode) {
                if (grid_down_cnt != grid_down_cnt_ || grid_right_cnt != grid_right_cnt_ || grid_depth_cnt != grid_depth_cnt_) {
                    throw std::runtime_error { "Count mismatch" };
                }
            }
            return {};
        }
    };
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
