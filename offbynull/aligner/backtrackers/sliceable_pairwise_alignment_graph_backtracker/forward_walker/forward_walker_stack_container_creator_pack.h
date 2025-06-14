#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_container_stack_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker
        ::forward_walker_stack_container_creator_pack {
    // using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_stack_container_creator_pack::row_slot_container_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_stack_container_creator_pack::resident_slot_container_stack_container_creator_pack;
    using offbynull::aligner::concepts::weight;

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
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_STACK_CONTAINER_CREATOR_PACK_H
