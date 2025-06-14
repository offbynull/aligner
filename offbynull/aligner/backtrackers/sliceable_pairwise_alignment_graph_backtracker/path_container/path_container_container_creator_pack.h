#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_CONTAINER_CREATOR_PACK_H

#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include <cstddef>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::path_container_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::unqualified_object_type;

    template<
        typename T,
        typename E
    >
    concept path_container_container_creator_pack =
        unqualified_object_type<T>
        && backtrackable_edge<E>
        && requires(const T t, std::size_t path_edge_capacity) {
            { t.create_element_container(path_edge_capacity) } -> random_access_range_of_type<element<E>>;
        };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_CONTAINER_CREATOR_PACK_H
