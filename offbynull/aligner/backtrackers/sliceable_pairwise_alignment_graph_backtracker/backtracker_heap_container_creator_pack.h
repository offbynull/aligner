#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_HEAP_CONTAINER_CREATOR_PACK_H

#include <vector>
#include <cstddef>
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_segmenter/resident_segmenter_heap_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/sliced_subdivider/sliced_subdivider_heap_container_creator_pack.h"
#include "offbynull/aligner/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtracker_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_segmenter
        ::resident_segmenter_heap_container_creator_pack::resident_segmenter_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::sliced_subdivider
        ::sliced_subdivider_heap_container_creator_pack::sliced_subdivider_heap_container_creator_pack;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        bool minimize_allocations
    >
    struct backtracker_heap_container_creator_pack {
        resident_segmenter_heap_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            minimize_allocations
        > create_resident_segmenter_container_creator_pack() {
            return {};
        }

        sliced_subdivider_heap_container_creator_pack<
            debug_mode,
            N,
            E,
            ED
        > create_sliced_subdivider_container_creator_pack() {
            return {};
        }

        std::vector<E> create_path_container(std::size_t path_edge_capacity) {
            std::vector<E> ret {};
            if constexpr (minimize_allocations) {
                ret.reserve(path_edge_capacity);
            }
            return ret;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_HEAP_CONTAINER_CREATOR_PACK_H
