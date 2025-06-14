#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_HEAP_CONTAINER_CREATOR_PACK_H

#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_container.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker
        ::forward_walker_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_heap_container_creator_pack::row_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_heap_container_creator_pack::resident_slot_container_heap_container_creator_pack;
    using offbynull::aligner::concepts::weight;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        bool minimize_allocations
    >
    struct forward_walker_heap_container_creator_pack {
        row_slot_container_heap_container_creator_pack<
            debug_mode,
            E,
            ED
        > create_row_slot_container_container_creator_pack() const {
            return {};
        }

        resident_slot_container_heap_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            minimize_allocations
        > create_resident_slot_container_container_creator_pack() const {
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_HEAP_CONTAINER_CREATOR_PACK_H
