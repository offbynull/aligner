#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_COMPARATOR_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_COMPARATOR_H

#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot.h"
#include "offbynull/aligner/concepts.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_comparator {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot;

    /**
     * @ref offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot comparator that compares the nodes of two slots
     * against each other using the less-than operator.
     *
     * @tparam N Graph node type.
     * @tparam E Graph edge type.
     * @tparam WEIGHT Graph edge's weight type.
     */
    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct slot_comparator {
        /**
         * Compare two @ref offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot "slot"'s by applying the
         * less-than operator on their nodes.
         *
         * @param lhs Left-hand side.
         * @param rhs Right-hand side.
         * @return `lhs.node < rhs.node`
         */
        bool operator()(const slot<N, E, WEIGHT>& lhs, const slot<N, E, WEIGHT>& rhs) const {
            return lhs.node < rhs.node;
        }

        /**
         * Compare an @ref offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot "slot"'s node against a node
         * object, by applying the less-than operator.
         *
         * @param lhs Left-hand side.
         * @param rhs Right-hand side.
         * @return `lhs.node < rhs`
         */
        bool operator()(const slot<N, E, WEIGHT>& lhs, const N& rhs) const {
            return lhs.node < rhs;
        }


        /**
         * Compare an @ref offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot "slot"'s node against a node
         * object, by applying the less-than operator.
         *
         * @param lhs Left-hand side.
         * @param rhs Right-hand side.
         * @return `lhs < rhs.node`
         */
        bool operator()(const N& lhs, const slot<N, E, WEIGHT>& rhs) const {
            return lhs < rhs.node;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_COMPARATOR_H
