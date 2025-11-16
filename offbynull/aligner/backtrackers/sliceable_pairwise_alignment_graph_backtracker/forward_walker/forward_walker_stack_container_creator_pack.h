#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_container_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/forward_walker/unimplemented_forward_walker_container_creator_pack.h"

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

    /**
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::forward_walker_container_creator_pack::forward_walker_container_creator_pack
     * that allocates its containers on the heap.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam N Graph node type.
     * @tparam E Graph edge type.
     * @tparam ED Graph edge data type (edge weight).
     * @tparam grid_right_cnt Expected right dimension of the underlying sliceable pairwise alignment graph instance.
     * @tparam grid_depth_cnt Expected depth dimension of the underlying sliceable pairwise alignment graph instance.
     * @tparam resident_nodes_capacity Of all nodes within the underlying pairwise alignment graph, the maximum number of nodes that are
     *      resident nodes (can be higher than the maximum, but not lower).
     */
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
        /**
         * @copydoc offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::unimplemented_forward_walker_container_creator_pack::unimplemented_forward_walker_container_creator_pack::create_row_slot_container_container_creator_pack
         */
        row_slot_container_stack_container_creator_pack<
            debug_mode,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt
        > create_row_slot_container_container_creator_pack() const {
            return {};
        }

        /**
         * @copydoc offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::unimplemented_forward_walker_container_creator_pack::unimplemented_forward_walker_container_creator_pack::create_resident_slot_container_container_creator_pack
         */
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
