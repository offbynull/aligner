#ifndef COMMON_ALLOCATORS_H
#define COMMON_ALLOCATORS_H

#include <stdexcept>
#include <cstddef>
#include <vector>
#include "boost/container/small_vector.hpp"
#include "boost/container/static_vector.hpp"

namespace offbynull::aligner::backtrack::common_allocators {
    template<typename ELEM, bool error_check = true>
    class VectorAllocator {
    public:
        std::vector<ELEM> allocate(size_t cnt) {
            return std::vector<ELEM>(cnt);
        }

        std::vector<ELEM> allocate(auto& begin, auto& end) {
            return std::vector<ELEM>(begin, end);
        }
    };

    template<typename ELEM, size_t size, bool error_check = true>
    class ArrayAllocator {
    public:
        std::array<ELEM, size> allocate(size_t cnt) {
            if constexpr (error_check) {
                if (cnt != size) {
                    throw std::runtime_error("Unexpected number of elements");
                }
            }
            return std::array<ELEM, size>{};
        }

        std::array<ELEM, size> allocate(auto& begin, auto& end) {
            return std::array<ELEM, size>(begin, end);
        }
    };

    template<typename ELEM, size_t max_size, bool error_check = true>
    class StaticVectorAllocator {
    public:
        boost::container::static_vector<ELEM, max_size> allocate(size_t cnt) {
            if constexpr (error_check) {
                if (cnt > max_size) {
                    throw std::runtime_error("Too many elements");
                }
            }
            return boost::container::static_vector<ELEM, max_size>(max_size);
        }

        boost::container::static_vector<ELEM, max_size> allocate(auto& begin, auto& end) {
            if constexpr (error_check) {
                auto cnt { end - begin };
                if (cnt > max_size) {
                    throw std::runtime_error("Too many elements");
                }
            }
            return boost::container::static_vector<ELEM, max_size>(begin, end);
        }
    };

    template<typename ELEM, size_t max_stack_size, bool error_check = true>
    class SmallVectorAllocator {
    public:
        boost::container::small_vector<ELEM, max_stack_size> allocate(size_t cnt) {
            return boost::container::small_vector<ELEM, max_stack_size>(cnt);
        }

        boost::container::small_vector<ELEM, max_stack_size> allocate(auto& begin, auto& end) {
            return boost::container::small_vector<ELEM, max_stack_size>(begin, end);
        }
    };
}

#endif //COMMON_ALLOCATORS_H
