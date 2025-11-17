#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_UNIMPLEMENTED_PATH_CONTAINER_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_UNIMPLEMENTED_PATH_CONTAINER_CONTAINER_CREATOR_PACK_H

#include <cstddef>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::unimplemented_path_container_container_creator_pack {
    /**
     * Unimplemented
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::path_container_container_creator_pack::path_container_container_creator_pack,
     * intended for documentation.
     */
    struct unimplemented_path_container_container_creator_pack {
        /**
         * Create random access container used to store elements (edges).
         *
         * @param path_edge_capacity Maximum number of elements the container can hold.
         * @return Empty random access container able to hold at least `path_edge_capacity` elements.
         */
        auto create_element_container(std::size_t path_edge_capacity) const;
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_UNIMPLEMENTED_PATH_CONTAINER_CONTAINER_CREATOR_PACK_H