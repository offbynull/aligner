#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLICED_SUBDIVIDER_SLICED_SUBDIVIDER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLICED_SUBDIVIDER_SLICED_SUBDIVIDER_HEAP_CONTAINER_CREATOR_PACK_H

#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/path_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/path_container_heap_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container_heap_container_creator_pack.h"
#include "offbynull/aligner/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::sliced_subdivider
        ::sliced_subdivider_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::path_container_heap_container_creator_pack::path_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_heap_container_creator_pack::row_slot_container_heap_container_creator_pack;
    using offbynull::aligner::concepts::weight;

    /**
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::sliced_subdivider::sliced_subdivider_container_creator_pack::sliced_subdivider_container_creator_pack
     * that allocates its containers on the heap.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam E Graph edge type.
     * @tparam ED Graph edge data type (edge weight).
     */
    template<
        bool debug_mode,
        backtrackable_edge E,
        weight ED
    >
    struct sliced_subdivider_heap_container_creator_pack {
        row_slot_container_heap_container_creator_pack<
            debug_mode,
            E,
            ED
        > create_row_slot_container_container_creator_pack() const {
            return {};
        }

        path_container_heap_container_creator_pack<
            debug_mode,
            E
        > create_path_container_container_creator_pack() const {
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLICED_SUBDIVIDER_SLICED_SUBDIVIDER_HEAP_CONTAINER_CREATOR_PACK_H
