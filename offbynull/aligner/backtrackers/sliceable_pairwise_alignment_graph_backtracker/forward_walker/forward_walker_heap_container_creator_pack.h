#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_FORWARD_WALKER_FORWARD_WALKER_HEAP_CONTAINER_CREATOR_PACK_H

#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/forward_walker/unimplemented_forward_walker_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker
        ::forward_walker_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_heap_container_creator_pack::row_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_heap_container_creator_pack::resident_slot_container_heap_container_creator_pack;
    using offbynull::aligner::concepts::weight;

    /**
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::forward_walker_container_creator_pack::forward_walker_container_creator_pack
     * that allocates its containers on the heap.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam N Graph node type.
     * @tparam E Graph edge type.
     * @tparam ED Graph edge data type (edge weight).
     * @tparam minimize_allocations `true` to force `create_resident_slot_container_container_creator_pack()` to create
     *     @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_container_heap_container_creator_pack:resident_slot_container_heap_container_creator_pack
     *     that's primed with the exact number of resident slots, thereby removing/reducing the need for adhoc reallocations.
     */
    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        bool minimize_allocations
    >
    struct forward_walker_heap_container_creator_pack {
        /**
         * @copydoc offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::unimplemented_forward_walker_container_creator_pack::unimplemented_forward_walker_container_creator_pack::create_row_slot_container_container_creator_pack
         */
        row_slot_container_heap_container_creator_pack<
            debug_mode,
            E,
            ED
        > create_row_slot_container_container_creator_pack() const {
            return {};
        }

        /**
         * @copydoc offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::unimplemented_forward_walker_container_creator_pack::unimplemented_forward_walker_container_creator_pack::create_resident_slot_container_container_creator_pack
         */
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
