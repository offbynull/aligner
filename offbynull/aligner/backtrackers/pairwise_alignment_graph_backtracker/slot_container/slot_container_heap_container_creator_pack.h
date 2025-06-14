#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <vector>
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/slot_container/slot.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container
        ::slot_container_heap_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot::slot;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        widenable_to_size_t PARENT_COUNT
    >
    struct slot_container_heap_container_creator_pack {
        std::vector<slot<N, E, ED, PARENT_COUNT>> create_slot_container(
            std::size_t grid_down_cnt,
            std::size_t grid_right_cnt,
            std::size_t grid_depth_cnt
        ) const {
            std::size_t cnt { (grid_down_cnt * grid_right_cnt) * grid_depth_cnt };
            return std::vector<slot<N, E, ED, PARENT_COUNT>>(cnt);
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H
