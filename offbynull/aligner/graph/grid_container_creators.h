#ifndef OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATORS_H
#define OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATORS_H

#include <vector>
#include <array>
#include "boost/container/small_vector.hpp"
#include "boost/container/static_vector.hpp"
#include "offbynull/aligner/graph/grid_container_creator.h"


namespace offbynull::aligner::graph::grid_container_creators {
    using offbynull::aligner::graph::grid_container_creator::grid_container_creator;

    template<typename ELEM_, typename T, bool error_check = true>
        requires std::is_integral_v<T> && std::is_unsigned_v<T>
    class vector_grid_container_creator {
    public:
        using ELEM = ELEM_;
        std::vector<ELEM> create_objects(T down_node_cnt, T right_node_cnt) {
            return std::vector<ELEM>(down_node_cnt * right_node_cnt);
        }
    };
    static_assert(grid_container_creator<vector_grid_container_creator<int, size_t>, size_t>);  // Sanity check

    template<typename ELEM_, typename T, T STATIC_DOWN_CNT, T STATIC_RIGHT_CNT, bool error_check = true>
        requires std::is_integral_v<T> && std::is_unsigned_v<T>
    class array_grid_container_creator {
    private:
        static constexpr T size = STATIC_DOWN_CNT * STATIC_RIGHT_CNT;
    public:
        using ELEM = ELEM_;
        std::array<ELEM, size> create_objects(T down_node_cnt, T right_node_cnt) {
            if constexpr (error_check) {
                if (down_node_cnt != STATIC_DOWN_CNT || right_node_cnt != STATIC_RIGHT_CNT) {
                    throw std::runtime_error("Unexpected number of elements");
                }
            }
            return std::array<ELEM, size>{};
        }
    };
    static_assert(grid_container_creator<array_grid_container_creator<int, size_t, 0u, 0u>, size_t>);  // Sanity check

    template<typename ELEM_, typename T, T STATIC_DOWN_CNT, T STATIC_RIGHT_CNT, bool error_check = true>
        requires std::is_integral_v<T> && std::is_unsigned_v<T>
    class static_vector_grid_container_creator {
    private:
        static constexpr T max_size = STATIC_DOWN_CNT * STATIC_RIGHT_CNT;
    public:
        using ELEM = ELEM_;
        boost::container::static_vector<ELEM, max_size> create_objects(T down_node_cnt, T right_node_cnt) {
            if constexpr (error_check) {
                if (down_node_cnt > STATIC_DOWN_CNT || right_node_cnt > STATIC_RIGHT_CNT) {
                    throw std::runtime_error("Too many elements");
                }
            }
            return boost::container::static_vector<ELEM, max_size>(down_node_cnt * right_node_cnt);
        }
    };
    static_assert(grid_container_creator<static_vector_grid_container_creator<int, size_t, 0u, 0u>, size_t>);  // Sanity check

    template<typename ELEM_, typename T, T STATIC_DOWN_CNT, T STATIC_RIGHT_CNT, bool error_check = true>
        requires std::is_integral_v<T> && std::is_unsigned_v<T>
    class small_vector_grid_container_creator {
    private:
        static constexpr T max_stack_size = STATIC_DOWN_CNT * STATIC_RIGHT_CNT;
    public:
        using ELEM = ELEM_;
        boost::container::small_vector<ELEM, max_stack_size> create_objects(T down_node_cnt, T right_node_cnt) {
            return boost::container::small_vector<ELEM, max_stack_size>(down_node_cnt * right_node_cnt);
        }
    };
    static_assert(grid_container_creator<small_vector_grid_container_creator<int, size_t, 0u, 0u>, size_t>);  // Sanity check
}

#endif //OFFBYNULL_ALIGNER_GRAPH_GRID_CONTAINER_CREATORS_H
