#ifndef OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATOR_H
#define OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATOR_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::graph::grid_container_creator {
    using offbynull::concepts::random_access_range_of_type;

    template <typename T, typename SIZE_T>
    concept grid_container_creator =
        std::unsigned_integral<SIZE_T> &&
        requires(T t, SIZE_T size) {
            typename T::ELEM;
            { t.create_objects(size, size) } -> random_access_range_of_type<typename T::ELEM>;
        };
}

#endif //OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATOR_H
