#ifndef PAIRWISE_GLOBAL_ALIGNMENT_GRAPH_H
#define PAIRWISE_GLOBAL_ALIGNMENT_GRAPH_H

#include <ranges>
#include <tuple>
#include <stdexcept>
#include <format>
#include <utility>
#include <vector>
#include <functional>
#include "grid_graph_4.h"
#include "utils.h"

namespace offbynull::pairwise_aligner::global {
    template<typename _ED, typename T = unsigned int>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    class node_data {
    private:
        using N = std::pair<T, T>;
        using E = std::pair<N, N>;
        using ED = _ED;
    public:
        E backtracking_edge;
        ED backtracking_weight;
    };

    template<typename _ED, typename _ND_CONTAINER, typename _ED_CONTAINER, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    class pairwise_global_alignment_graph {
    public:
        using N = std::pair<T, T>;
        using E = std::pair<N, N>;
        using ED = _ED;
        using ND = node_data<_ED, T>;

    private:
        offbynull::grid_graph::grid_graph::grid_graph<ND, _ED, _ND_CONTAINER, _ED_CONTAINER, T, error_check> g;

    public:
        pairwise_global_alignment_graph(
            T _down_node_cnt,
            T _right_node_cnt,
            std::function<_ND_CONTAINER(T, T)> nd_container_creator,
            std::function<_ED_CONTAINER(T, T)> ed_container_creator
        )
        : g{_down_node_cnt, _right_node_cnt, nd_container_creator, ed_container_creator} {}

        void update_node_data(const N& node, ND&& data) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            g.update_node_data(node, std::forward<ND>(data));
        }

        ND& get_node_data(const N& node) {
            return g.get_node_data(node);
        }

        void update_edge_data(const E& edge, ED&& data) {
            if constexpr (error_check) {
                if (!has_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            g.update_edge_data(edge.inner_edge, data);
        }

        ED& get_edge_data(const E& edge) {
            return std::get<2>(get_edge(edge));
        }

        N get_edge_from(const E& edge) {
            return std::get<0>(get_edge(edge));
        }

        N get_edge_to(const E& edge) {
            return std::get<1>(get_edge(edge));
        }

        std::tuple<N, N, ED&> get_edge(const E& edge) {
            if constexpr (error_check) {
                if (!has_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            return g.get_edge(edge.inner_edge);
        }

        auto get_root_nodes() {
            return g.get_root_nodes();
        }

        N get_root_node() {
            return g.get_root_node();
        }

        auto get_leaf_nodes() {
            return g.get_leaf_nodes();
        }

        auto get_nodes() {
            return g.get_nodes();
        }

        auto get_edges() {
            return g.get_edges();
        }

        bool has_node(const N& node) {
            return g.has_node(node);
        }

        bool has_edge(const E& edge) {
            return g.has_edge(edge);
        }

        auto get_outputs_full(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_outputs_full(node);
        }

        std::tuple<E, N, N, ED&> get_output_full(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_output_full(node);
        }

        auto get_inputs_full(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_inputs_full(node);
        }

        std::tuple<E, N, N, ED&> get_input_full(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_input_full(node);
        }

        auto get_outputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_outputs(node);
        }

        E get_output(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_output();
        }

        auto get_inputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_inputs(node);
        }

        E get_input(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_input(node);
        }

        bool has_outputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.has_outputs(node);
        }

        bool has_inputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.has_inputs(node);
        }

        size_t get_out_degree(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_out_degree(node);
        }

        size_t get_in_degree(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return g.get_in_degree(node);
        }
    };

    template<typename _ED, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    auto create_vector_grid(T down_cnt, T right_cnt) {
        size_t size = down_cnt * right_cnt;
        using ND = node_data<_ED, T>;
        return pairwise_global_alignment_graph<_ED, std::vector<ND>, std::vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>>, T, error_check> {
            down_cnt,
            right_cnt,
            [size](T, T) { return std::vector<ND>(size); },
            [size](T, T) { return std::vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>>(size); }
        };
    }

    template<typename _ED, size_t STATIC_DOWN_CNT, size_t STATIC_RIGHT_CNT, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    auto create_array_grid() {
        constexpr size_t size = STATIC_DOWN_CNT * STATIC_RIGHT_CNT;
        using ND = node_data<_ED, T>;
        return pairwise_global_alignment_graph<_ED, std::array<ND, size>, std::array<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, size>, T, error_check> {
            STATIC_DOWN_CNT,
            STATIC_RIGHT_CNT,
            [](T, T) { return std::array<ND, size>{}; },
            [](T, T) { return std::array<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, size> {}; }
        };
    }

    template<typename _ED, size_t STATIC_DOWN_CNT, size_t STATIC_RIGHT_CNT, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    auto create_small_vector_grid(T down_cnt, T right_cnt) {
        constexpr size_t stack_size = STATIC_DOWN_CNT * STATIC_RIGHT_CNT;
        size_t actual_size = down_cnt * right_cnt;
        using ND = node_data<_ED, T>;
        return pairwise_global_alignment_graph<_ED, boost::container::small_vector<ND, stack_size>, boost::container::small_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>, T, error_check> {
            down_cnt,
            right_cnt,
            [actual_size](T, T) { return boost::container::small_vector<ND, stack_size>(actual_size); },
            [actual_size](T, T) { return boost::container::small_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>(actual_size); }
        };
    }

    template<typename _ED, size_t STATIC_DOWN_CNT, size_t STATIC_RIGHT_CNT, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    auto create_static_vector_grid(T down_cnt, T right_cnt) {
        constexpr size_t stack_size = STATIC_DOWN_CNT * STATIC_RIGHT_CNT;
        size_t actual_size = down_cnt * right_cnt;
        using ND = node_data<_ED, T>;
        return pairwise_global_alignment_graph<_ED, boost::container::static_vector<ND, stack_size>, boost::container::static_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>, T, error_check> {
            down_cnt,
            right_cnt,
            [actual_size](T, T) { return boost::container::static_vector<ND, stack_size>(actual_size); },
            [actual_size](T, T) { return boost::container::static_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>(actual_size); }
        };
    }
}
#endif //PAIRWISE_GLOBAL_ALIGNMENT_GRAPH_H
