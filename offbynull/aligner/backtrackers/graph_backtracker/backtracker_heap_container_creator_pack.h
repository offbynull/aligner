#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_HEAP_CONTAINER_CREATOR_PACK_H

#include <vector>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_heap_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot_container_heap_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::backtracker_heap_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_heap_container_creator_pack
        ::slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_heap_container_creator_pack
        ::ready_queue_heap_container_creator_pack;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct backtracker_heap_container_creator_pack {
        slot_container_heap_container_creator_pack<
            debug_mode,
            N,
            E,
            WEIGHT
        > create_slot_container_container_creator_pack() const {
            return {};
        }

        ready_queue_heap_container_creator_pack<
            debug_mode
        > create_ready_queue_container_creator_pack() const {
            return {};
        }

        std::vector<E> create_path_container() const {
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_HEAP_CONTAINER_CREATOR_PACK_H
