#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H

#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include <cstddef>
#include <vector>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::path_container_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;

    template<
        bool debug_mode,
        backtrackable_edge E
    >
    struct path_container_heap_container_creator_pack {
        std::vector<element<E>> create_element_container(std::size_t path_edge_capacity) const {
            return std::vector<element<E>>(path_edge_capacity);
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H
