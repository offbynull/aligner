#ifndef OFFBYNULL_ALIGNER_BACKTRACK_ALLOCATOR_H
#define OFFBYNULL_ALIGNER_BACKTRACK_ALLOCATOR_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrack::allocator {
    using offbynull::concepts::random_access_range_of_type;

    // You can use unimplemented types as requires params -- the compiler will check to see if it has the same traits
    template<typename T>
    struct unimplemented_input_iterator {
        using difference_type = std::ptrdiff_t;
        using value_type = T;

        T operator*() const;
        unimplemented_input_iterator& operator++();
        void operator++(int) { ++*this; }
    };

    template <typename T>
    concept allocator =
        requires(T t, size_t size, unimplemented_input_iterator<typename T::ELEM> it) {
            typename T::ELEM;
            { t.allocate(size) } -> random_access_range_of_type<typename T::ELEM>;
            { t.allocate(it, it) } -> random_access_range_of_type<typename T::ELEM>;
        };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACK_ALLOCATOR_H
