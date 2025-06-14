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

    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct resident_slot_with_node_comparator {
        bool operator()(
            const resident_slot_with_node<N, E, WEIGHT>& lhs,
            const resident_slot_with_node<N, E, WEIGHT>& rhs
        ) const {
            return lhs.node < rhs.node;
        }

        bool operator()(
            const resident_slot_with_node<N, E, WEIGHT>& lhs,
            const N& rhs
        ) const {
            return lhs.node < rhs;
        }

        bool operator()(
            const N& lhs,
            const resident_slot_with_node<N, E, WEIGHT>& rhs
        ) const {
            return lhs < rhs.node;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_COMPARATOR_H
