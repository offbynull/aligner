#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_H

#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_with_node {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot
        ::resident_slot;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::concepts::weight;

    PACK_STRUCT_START
    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct resident_slot_with_node {
        N node;
        resident_slot<E, WEIGHT> slot_;
    }
    PACK_STRUCT_STOP;
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_WITH_NODE_H
