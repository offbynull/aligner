#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_ROW_ENTRY_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_ROW_ENTRY_H

#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/slot.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::row_entry {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container::slot::slot;
    using offbynull::aligner::concepts::weight;

    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct row_entry {
        N node;
        slot<E, WEIGHT>* slot_ptr;

        row_entry()
        : node {}
        , slot_ptr { nullptr } {}
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_ROW_ENTRY_H
