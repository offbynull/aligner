#ifndef OFFBYNULL_ALIGNER_GRAPH_GRID_ALLOCATOR_H
#define OFFBYNULL_ALIGNER_GRAPH_GRID_ALLOCATOR_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::graph::grid_allocator {
    using offbynull::concepts::random_access_range_of_type;

    template <typename T, typename SIZE_T>
    concept grid_allocator =
        std::unsigned_integral<SIZE_T> &&
        requires(T t, SIZE_T size) {
            typename T::ELEM;
            { t.allocate(size, size) } -> random_access_range_of_type<typename T::ELEM>;
        };
}

#endif //OFFBYNULL_ALIGNER_GRAPH_GRID_ALLOCATOR_H
