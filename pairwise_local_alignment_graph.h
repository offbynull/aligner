#ifndef PAIRWISE_LOCAL_ALIGNMENT_GRAPH_H
#define PAIRWISE_LOCAL_ALIGNMENT_GRAPH_H

#include <ranges>
#include <tuple>
#include <stdexcept>
#include <format>
#include <string>
#include <utility>
#include <vector>
#include <functional>
#include "grid_graph_4.h"
#include "utils.h"

namespace offbynull::pairwise_aligner::local {
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
    class pairwise_local_alignment_graph {
    public:
        using N = std::pair<T, T>;
        using E = std::pair<N, N>;
        using ED = _ED;
        using ND = node_data<_ED, T>;

    private:
        offbynull::grid_graph::grid_graph::grid_graph<ND, _ED, _ND_CONTAINER, _ED_CONTAINER, T, error_check> g;
        ED freeride_ed;

    public:
        pairwise_local_alignment_graph(
            T _down_node_cnt,
            T _right_node_cnt,
            std::function<_ND_CONTAINER(T, T)> nd_container_creator,
            std::function<_ED_CONTAINER(T, T)> ed_container_creator
        )
        : g{_down_node_cnt, _right_node_cnt, nd_container_creator, ed_container_creator}, freeride_ed{0.0} {}

        bool is_freeridge_edge(const E& edge) {
            if constexpr (error_check) {
                if (!is_valid_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            if (g.has_edge(edge)) {
                return false;
            }
            const auto & [n1, n2] { edge };
            const auto & [n1_down, n1_right] { n1 };
            const auto & [n2_down, n2_right] { n1 };
            return (n1_down == 0u && n1_right == 0u && n2_down > 0u && n2_right > 0u)
                || (n1_down < g.down_node_cnt - 1u && n1_right < g.right_node_cnt - 1u && n2_down == g.down_node_cnt - 1u && n2_right == g.right_node_cnt - 1u);
        }

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
                if (!is_valid_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            if (is_freeridge_edge(edge)) {
                freeride_ed = std::forward<ED>(data);
            } else {
                g.update_edge_data(edge, data);
            }
        }

        ED& get_edge_data(const E& edge) {
            if constexpr (error_check) {
                if (!is_valid_edge(edge)) {
                    throw std::runtime_error("Edge doesn't exist");
                }
            }
            return std::get<2>(get_edge());
        }

        N get_edge_from(const E& edge) {
            if constexpr (error_check) {
                if (!is_valid_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            return std::get<0>(get_edge());
        }

        N get_edge_to(const E& edge) {
            if constexpr (error_check) {
                if (!is_valid_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            return std::get<1>(get_edge());
        }

        std::tuple<N, N, ED&> get_edge(const E& edge) {
            if constexpr (error_check) {
                if (!is_valid_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            if (is_freeridge_edge(edge)) {
                auto [n1, n2] = edge;
                return std::tuple<N, N, ED&> {n1, n2, freeride_ed};
            } else {
                return g.get_edge(edge);
            }
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
            auto from_src_range {
                std::views::cartesian_product(
                    std::views::iota(0u, g.down_node_cnt),
                    std::views::iota(0u, g.right_node_cnt)
                )
                | std::views::drop(1) // drop 0,0
                | std::views::transform([&](const auto & p) noexcept {
                    N n1 { 0u, 0u };
                    N n2 { std::get<0>(p), std::get<1>(p) };
                    return E { n1, n2 };
                })
            };
            auto to_sink_range {
                std::views::cartesian_product(
                    std::views::iota(0u, g.down_node_cnt),
                    std::views::iota(0u, g.right_node_cnt)
                )
                | ( std::views::reverse | std::views::drop(1) | std::views::reverse ) // drop bottom right
                | std::views::transform([&](const auto & p) noexcept {
                    N n1 { std::get<0>(p), std::get<1>(p) };
                    N n2 { g.down_node_cnt, g.right_node_cnt };
                    return E { n1, n2 };
                })
            };
            auto real_range {
                g.get_edges()
            };
            // TypeDisplayer<decltype(*from_src_range.begin())> x1;
            // TypeDisplayer<decltype(*to_sink_range.begin())> x2;
            // TypeDisplayer<decltype(*g.get_edges().begin())> x3;
            // TypeDisplayer<decltype(std::begin(real_range))> x4;
            // TypeDisplayer<decltype(std::begin(from_src_range))> x5;
            // return concat_view {
            //     std::move(to_sink_range),
            //     std::move(from_src_range)
            // };
            return concat_view {
                std::move(real_range),
                concat_view { from_src_range, to_sink_range }
            };
            // return concat_view { from_src_range, to_sink_range };
        }

        bool has_node(const N& node) {
            return g.has_node(node);
        }

        bool has_edge(const E& edge) {
            return is_valid_edge(edge);
        }

        auto get_outputs_full(const N& node) {
            if (node == N{ g.down_node_cnt - 1u, g.right_node_cnt - 1u }) {
                return g.get_outputs(node);
            }
            return make_concat_view(
                g.get_outputs(node),
                std::views::single({ node, { g.down_node_cnt - 1u, g.right_node_cnt - 1u } })
            );
        }

        std::tuple<E, N, N, ED&> get_output_full(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            auto it = this->get_outputs_full(node).begin();
            const auto& ret = *it;
            ++it;
            if (it != this->get_outputs_full(node).end()) {
                throw std::runtime_error {"More than one edge exists"};
            }
            return ret;
        }

        auto get_inputs_full(const N& node) {
            if (node == N{ 0u, 0u }) {
                return g.get_inputs(node);
            }
            return make_concat_view(
                g.get_inputs(node),
                std::views::single({ { 0u, 0u }, node })
            );
        }

        std::tuple<E, N, N, ED&> get_input_full(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            auto it = this->get_inputs_full(node).begin();
            const auto& ret = *it;
            ++it;
            if (it != this->get_inputs_full(node).end()) {
                throw std::runtime_error {"More than one edge exists"};
            }
            return ret;
        }

        auto get_outputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return get_outputs_full(node) | std::views::transform([this](auto v) noexcept -> E { return std::get<0>(v); });
        }

        E get_output(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            auto it = this->get_outputs(node).begin();
            const auto& ret = *it;
            ++it;
            if (it != this->get_outputs_full(node).end()) {
                throw std::runtime_error {"More than one edge exists"};
            }
            return ret;
        }

        auto get_inputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return this->get_inputs_full(node) | std::views::transform([this](auto v) noexcept -> E { return std::get<0>(v); });
        }

        E get_input(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            auto it = this->get_inputs(node).begin();
            const auto& ret = *it;
            ++it;
            if (it != this->get_outputs_full(node).end()) {
                throw std::runtime_error {"More than one edge exists"};
            }
            return ret;
        }

        bool has_outputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return !this->get_outputs(node).empty();
        }

        bool has_inputs(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return !this->get_inputs(node).empty();
        }

        size_t get_out_degree(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return this->get_outputs(node).size();
        }

        size_t get_in_degree(const N& node) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            return this->get_inputs(node).size();
        }
    };

    template<typename _ED, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    auto create_vector_grid(T down_cnt, T right_cnt) {
        size_t size = down_cnt * right_cnt;
        using ND = node_data<_ED, T>;
        return pairwise_local_alignment_graph<_ED, std::vector<ND>, std::vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>>, T, error_check> {
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
        return pairwise_local_alignment_graph<_ED, std::array<ND, size>, std::array<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, size>, T, error_check> {
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
        return pairwise_local_alignment_graph<_ED, boost::container::small_vector<ND, stack_size>, boost::container::small_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>, T, error_check> {
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
        return pairwise_local_alignment_graph<_ED, boost::container::static_vector<ND, stack_size>, boost::container::static_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>, T, error_check> {
            down_cnt,
            right_cnt,
            [actual_size](T, T) { return boost::container::static_vector<ND, stack_size>(actual_size); },
            [actual_size](T, T) { return boost::container::static_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>(actual_size); }
        };
    }
}
#endif //PAIRWISE_LOCAL_ALIGNMENT_GRAPH_H
