#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <boost/container/small_vector.hpp>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot_container_stack_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::backtracker_stack_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_stack_container_creator_pack
        ::slot_container_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack
        ::ready_queue_stack_container_creator_pack;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT,
        std::size_t slot_container_heap_escape_size = 100zu,
        std::size_t ready_queue_heap_escape_size = 100zu,
        std::size_t path_container_heap_escape_size = 10zu
    >
    struct backtracker_stack_container_creator_pack {
        slot_container_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            WEIGHT,
            slot_container_heap_escape_size
        > create_slot_container_container_creator_pack() const {
            return {};
        }

        ready_queue_stack_container_creator_pack<
            debug_mode,
            ready_queue_heap_escape_size
        > create_ready_queue_container_creator_pack() const {
            return {};
        }

        boost::container::small_vector<
            E,
            path_container_heap_escape_size
        > create_path_container() const {
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
