#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BIDI_WALKER_BIDI_WALKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BIDI_WALKER_BIDI_WALKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/forward_walker/forward_walker_stack_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::bidi_walker
        ::bidi_walker_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker
        ::forward_walker_stack_container_creator_pack::forward_walker_stack_container_creator_pack;
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
    struct bidi_walker_stack_container_creator_pack {
        forward_walker_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt,
            resident_nodes_capacity
        > create_forward_walker_container_creator_pack() {
            return {};
        }

        forward_walker_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt,
             resident_nodes_capacity
        > create_backward_walker_container_creator_pack() {
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BIDI_WALKER_BIDI_WALKER_STACK_CONTAINER_CREATOR_PACK_H
