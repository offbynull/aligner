#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <vector>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::ready_queue::ready_queue_heap_container_creator_pack {
    using offbynull::concepts::widenable_to_size_t;

    template<
        bool debug_mode,
        widenable_to_size_t SLOT_INDEX,
        bool minimize_allocations
    >
    struct ready_queue_heap_container_creator_pack {
        std::vector<SLOT_INDEX> create_queue_container(
            std::size_t grid_down_cnt,
            std::size_t grid_right_cnt,
            std::size_t grid_depth_cnt
        ) const {
            std::vector<SLOT_INDEX> ret {};
            if constexpr (minimize_allocations) {
                ret.reserve(grid_down_cnt * grid_right_cnt * grid_depth_cnt);
            }
            return ret;
        }
    };
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H
