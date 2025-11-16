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

    // TODO: Should this struct be packable via OBN_PACK_STRUCTS? Probably not, because there's not that many copies of it in memory.

    /**
     * Current node / slot being walked by an
     * @link offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::forward_walker::forward_walker @endlink.
     *
     * @tparam N Graph node type.
     * @tparam E Graph edge type.
     * @tparam WEIGHT Graph edge data type (edge weight).
     */
    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct row_entry {
        /** Node. */
        N node;
        /** Slot for `node`. */
        slot<E, WEIGHT>* slot_ptr;

        /**
         * Construct a default
         * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::row_entry::row_entry
         * instance.
         */
        row_entry()
        : node {}
        , slot_ptr { nullptr } {}
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_ROW_ENTRY_H
