#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_COMPARATOR_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_COMPARATOR_H

#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_with_node.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_with_node_comparator {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_with_node::resident_slot_with_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::concepts::weight;

    /**
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_with_node::resident_slot_with_node
     * comparator that compares the nodes of two slots against each other using the less-than operator.
     *
     * @tparam N Graph node identifier type.
     * @tparam E Graph edge identifier type.
     * @tparam ED Graph edge data type (edge weight).
     */
    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight ED
    >
    struct resident_slot_with_node_comparator {
        /**
         * Compare two
         * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_with_node::resident_slot_with_node "slot"'s
         * by applying the less-than operator on their nodes.
         *
         * @param lhs Left-hand side.
         * @param rhs Right-hand side.
         * @return `lhs.node < rhs.node`
         */
        bool operator()(
            const resident_slot_with_node<N, E, ED>& lhs,
            const resident_slot_with_node<N, E, ED>& rhs
        ) const {
            return lhs.node < rhs.node;
        }

        /**
         * Compare an
         * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_with_node::resident_slot_with_node "slot"'s
         * node identifier against a node identifier, by applying the less-than operator.
         *
         * @param lhs Left-hand side.
         * @param rhs Right-hand side.
         * @return `lhs.node < rhs`
         */
        bool operator()(
            const resident_slot_with_node<N, E, ED>& lhs,
            const N& rhs
        ) const {
            return lhs.node < rhs;
        }

        /**
         * Compare an
         * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_with_node::resident_slot_with_node "slot"'s
         * node identifier against a node identifier, by applying the less-than operator.
         *
         * @param lhs Left-hand side.
         * @param rhs Right-hand side.
         * @return `lhs < rhs.node`
         */
        bool operator()(
            const N& lhs,
            const resident_slot_with_node<N, E, ED>& rhs
        ) const {
            return lhs < rhs.node;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_COMPARATOR_H
