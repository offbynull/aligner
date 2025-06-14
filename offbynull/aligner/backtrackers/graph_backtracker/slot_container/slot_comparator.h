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

    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct slot_comparator {
        bool operator()(const slot<N, E, WEIGHT>& lhs, const slot<N, E, WEIGHT>& rhs) const {
            return lhs.node < rhs.node;
        }

        bool operator()(const slot<N, E, WEIGHT>& lhs, const N& rhs) const {
            return lhs.node < rhs;
        }

        bool operator()(const N& lhs, const slot<N, E, WEIGHT>& rhs) const {
            return lhs < rhs.node;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_COMPARATOR_H
