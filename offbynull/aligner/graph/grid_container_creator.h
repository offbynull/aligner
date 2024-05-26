#ifndef OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATOR_H
#define OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATOR_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::graph::grid_container_creator {
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::widenable_to_size_t;

    template <typename T, typename INDEX>
    concept grid_container_creator =
        widenable_to_size_t<INDEX>
        && requires(T t, INDEX size) {
            typename T::ELEM;
            { t.create_objects(size, size) } -> random_access_range_of_type<typename T::ELEM>;
        };
}

#endif //OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATOR_H
