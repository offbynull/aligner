#ifndef PAIRWISE_EXTENDED_GAP_ALIGNMENT_GRAPH_H
#define PAIRWISE_EXTENDED_GAP_ALIGNMENT_GRAPH_H

#include <ranges>
#include <tuple>
#include <stdexcept>
#include <format>
#include <utility>
#include <vector>
#include <functional>
#include "grid_graph_diagonal.h"
#include "grid_graph_down.h"
#include "grid_graph_right.h"
#include "utils.h"

namespace offbynull::pairwise_aligner::extended_gap {
    template<typename _ND, typename _ND_CONTAINER, typename T = unsigned int, bool error_check = true>
        requires std::is_integral_v<T> && std::is_unsigned_v<T>
    class shifted_grid_graph_right {
    public:
        using N = std::pair<T, T>;
        using ND = _ND;
        using E = std::pair<N, N>;
        using ED = std::tuple<>;  // empty obj

    private:
        offbynull::grid_graph::grid_graph_right::grid_graph_right<ND, ED, _ND_CONTAINER, std::array<ED, 0>, T, error_check> g;

        static N to_raw_node(const N& node) {
            const auto& [n_down, n_right] { node };
            return { n_down, n_right - 1u };
        }

        static N from_raw_node(const N& node) {
            const auto& [n_down, n_right] { node };
            return { n_down, n_right + 1u };
        }

        static E to_raw_edge(const E& node) {
            const auto& [n1, n2] { node };
            return { to_raw_node(n1), to_raw_node(n2) };
        }

        static E from_raw_edge(const E& node) {
            const auto& [n1, n2] { node };
            return { from_raw_node(n1), from_raw_node(n2) };
        }

    public:
        shifted_grid_graph_right(
            T _down_node_max,
            T _right_node_max,
            std::function<_ND_CONTAINER(T, T)> nd_container_creator
        )
        : g{_down_node_max, _right_node_max - 1u, nd_container_creator, [](T, T) { return std::array<ED, 0>{}; } } {}

        void update_node_data(const N& node, ND&& data) {
            if constexpr (error_check) {
                if (!has_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            ed = std::forward<ED>(data);
        }

        ND& get_node_data(const N& node) {
            return g.get_node_data(to_raw_node(node));
        }

        void update_edge_data(const E& edge, ED&& data) {
            return g.update_edge_data(to_raw_edge(edge), data);
        }

        ED& get_edge_data(const E& edge) {
            return g.get_edge_data(to_raw_edge(edge));
        }

        N get_edge_from(const E& edge) {
            return g.get_edge_from(to_raw_edge(edge));
        }

        N get_edge_to(const E& edge) {
            return g.get_edge_from(to_raw_edge(edge));
        }

        std::tuple<N, N, ED&> get_edge(const E& edge) {
            return g.get_edge(to_raw_edge(edge));
        }

        auto get_root_nodes() {
            return g.get_root_nodes() | std::views::transform([](const auto& n) noexcept { return from_raw_node(n); });
        }

        N get_root_node() {
            return from_raw_node(g.get_root_node());
        }

        auto get_leaf_nodes() {
            return g.get_leaf_nodes() | std::views::transform([](const auto& n) noexcept { return from_raw_node(n); });
        }

        auto get_nodes() {
            return g.get_nodes() | std::views::transform([](const auto& n) noexcept { return from_raw_node(n); });
        }

        auto get_edges() {
            return g.get_edges() | std::views::transform([](const auto& e) noexcept { return from_raw_edge(e); });
        }

        bool has_node(const N& node) {
            return g.has_node(to_raw_node(node));
        }

        bool has_edge(const E& edge) {
            return g.has_node(to_raw_edge(edge));
        }

        auto get_outputs_full(const N& node) {
            return g.get_outputs_full(to_raw_node(node))
                | std::views::transform([](const auto& e_full) noexcept {
                    auto& [e, n1, n2, ed] { e_full };
                    return std::tuple<E, N, N, ED&> {
                        from_raw_edge(e),
                        from_raw_node(n1),
                        from_raw_node(n2),
                        ed
                    };
                });
        }

        std::tuple<E, N, N, ED&> get_output_full(const N& node) {
            return g.get_output_full(to_raw_node(node));
        }

        auto get_inputs_full(const N& node) {
            return g.get_inputs_full(to_raw_node(node))
                | std::views::transform([](const auto& e_full) noexcept {
                    auto& [e, n1, n2, ed] { e_full };
                    return std::tuple<E, N, N, ED&> {
                        from_raw_edge(e),
                        from_raw_node(n1),
                        from_raw_node(n2),
                        ed
                    };
                });
        }

        std::tuple<const E&, const N&, const N&, ED&> get_input_full(const N& node) {
            return g.get_input_full(to_raw_node(node));
        }

        auto get_outputs(const N& node) {
            return g.get_outputs(to_raw_node(node))
                | std::views::transform([](const auto& e) noexcept { return from_raw_edge(e); });
        }

        E get_output(const N& node) {
            return from_raw_edge(g.get_output(to_raw_node(node)));
        }

        auto get_inputs(const N& node) {
            return g.get_inputs(to_raw_node(node))
                | std::views::transform([](const auto& e) noexcept { return from_raw_edge(e); });
        }

        E get_input(const N& node) {
            return from_raw_edge(g.get_input(to_raw_node(node)));
        }

        bool has_outputs(const N& node) {
            return g.has_outputs(to_raw_node(node));
        }

        bool has_inputs(const N& node) {
            return g.has_inputs(to_raw_node(node));
        }

        size_t get_out_degree(const N& node) {
            return g.get_out_degree(to_raw_node(node));
        }

        size_t get_in_degree(const N& node) {
            return g.get_in_degree(to_raw_node(node));
        }

        std::string to_string() {
            std::string out {};
            for (const N& node : this->get_nodes()) {
                out += std::format("node {}: {}\n", node, this->get_node_data(node));
                for (const E& edge : this->get_outputs(node)) {
                    auto [from_node, to_node, edge_data] = this->get_edge(edge);
                    out += std::format("  edge {} pointing to node {}: {}\n", edge, to_node, edge_data);
                }
            }
            return out;
        }
    };




    template<typename _ND, typename _ND_CONTAINER, typename T = unsigned int, bool error_check = true>
        requires std::is_integral_v<T> && std::is_unsigned_v<T>
    class shifted_grid_graph_down {
    public:
        using N = std::pair<T, T>;
        using ND = _ND;
        using E = std::pair<N, N>;
        using ED = std::tuple<>;  // empty obj

    private:
        offbynull::grid_graph::grid_graph_down::grid_graph_down<ND, ED, _ND_CONTAINER, std::array<_ED, 0>, T, error_check> g;

        static N to_raw_node(const N& node) {
            const auto& [n_down, n_right] { node };
            return { n_down - 1u, n_right };
        }

        static N from_raw_node(const N& node) {
            const auto& [n_down, n_right] { node };
            return { n_down + 1u, n_right };
        }

        static E to_raw_edge(const E& node) {
            const auto& [n1, n2] { node };
            return { to_raw_node(n1), to_raw_node(n2) };
        }

        static E from_raw_edge(const E& node) {
            const auto& [n1, n2] { node };
            return { from_raw_node(n1), from_raw_node(n2) };
        }

    public:
        shifted_grid_graph_down(
            T _down_node_max,
            T _right_node_max,
            std::function<_ND_CONTAINER(T, T)> nd_container_creator
        )
        : g{_down_node_max - 1u, _right_node_max, nd_container_creator, [](T, T) { return std::array<ED, 0>{}; } } {}

        void update_node_data(const N& node, ND&& data) {
            g.update_node_data(to_raw_node(node), data);
        }

        ND& get_node_data(const N& node) {
            return g.get_node_data(to_raw_node(node));
        }

        void update_edge_data(const E& edge, ED&& data) {
            return g.update_edge_data(to_raw_edge(edge), data);
        }

        ED& get_edge_data(const E& edge) {
            return g.get_edge_data(to_raw_edge(edge));
        }

        N get_edge_from(const E& edge) {
            return g.get_edge_from(to_raw_edge(edge));
        }

        N get_edge_to(const E& edge) {
            return g.get_edge_from(to_raw_edge(edge));
        }

        std::tuple<N, N, ED&> get_edge(const E& edge) {
            return g.get_edge(to_raw_edge(edge));
        }

        auto get_root_nodes() {
            return g.get_root_nodes() | std::views::transform([](const auto& n) noexcept { return from_raw_node(n); });
        }

        N get_root_node() {
            return from_raw_node(g.get_root_node());
        }

        auto get_leaf_nodes() {
            return g.get_leaf_nodes() | std::views::transform([](const auto& n) noexcept { return from_raw_node(n); });
        }

        auto get_nodes() {
            return g.get_nodes() | std::views::transform([](const auto& n) noexcept { return from_raw_node(n); });
        }

        auto get_edges() {
            return g.get_edges() | std::views::transform([](const auto& e) noexcept { return from_raw_edge(e); });
        }

        bool has_node(const N& node) {
            return g.has_node(to_raw_node(node));
        }

        bool has_edge(const E& edge) {
            return g.has_node(to_raw_edge(edge));
        }

        auto get_outputs_full(const N& node) {
            return g.get_outputs_full(to_raw_node(node))
                | std::views::transform([](const auto& e_full) noexcept {
                    auto& [e, n1, n2, ed] { e_full };
                    return std::tuple<E, N, N, ED&> {
                        from_raw_edge(e),
                        from_raw_node(n1),
                        from_raw_node(n2),
                        ed
                    };
                });
        }

        std::tuple<E, N, N, ED&> get_output_full(const N& node) {
            return g.get_output_full(to_raw_node(node));
        }

        auto get_inputs_full(const N& node) {
            return g.get_inputs_full(to_raw_node(node))
                | std::views::transform([](const auto& e_full) noexcept {
                    auto& [e, n1, n2, ed] { e_full };
                    return std::tuple<E, N, N, ED&> {
                        from_raw_edge(e),
                        from_raw_node(n1),
                        from_raw_node(n2),
                        ed
                    };
                });
        }

        std::tuple<const E&, const N&, const N&, ED&> get_input_full(const N& node) {
            return g.get_input_full(to_raw_node(node));
        }

        auto get_outputs(const N& node) {
            return g.get_outputs(to_raw_node(node))
                | std::views::transform([](const auto& e) noexcept { return from_raw_edge(e); });
        }

        E get_output(const N& node) {
            return from_raw_edge(g.get_output(to_raw_node(node)));
        }

        auto get_inputs(const N& node) {
            return g.get_inputs(to_raw_node(node))
                | std::views::transform([](const auto& e) noexcept { return from_raw_edge(e); });
        }

        E get_input(const N& node) {
            return from_raw_edge(g.get_input(to_raw_node(node)));
        }

        bool has_outputs(const N& node) {
            return g.has_outputs(to_raw_node(node));
        }

        bool has_inputs(const N& node) {
            return g.has_inputs(to_raw_node(node));
        }

        size_t get_out_degree(const N& node) {
            return g.get_out_degree(to_raw_node(node));
        }

        size_t get_in_degree(const N& node) {
            return g.get_in_degree(to_raw_node(node));
        }

        std::string to_string() {
            std::string out {};
            for (const N& node : this->get_nodes()) {
                out += std::format("node {}: {}\n", node, this->get_node_data(node));
                for (const E& edge : this->get_outputs(node)) {
                    auto [from_node, to_node, edge_data] = this->get_edge(edge);
                    out += std::format("  edge {} pointing to node {}: {}\n", edge, to_node, edge_data);
                }
            }
            return out;
        }
    };









    enum class node_type : uint8_t {
        DIAGONAL,
        DOWN,
        RIGHT
    };

    template<typename T = unsigned int>
    class node {
    public:
        using N = std::pair<T, T>;
        node_type type;
        N inner_node;

        std::strong_ordering operator<=>(const node& rhs) const = default;
    };

    template<typename _ED, typename T = unsigned int>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    class node_data {
    private:
        using N = node<T>;
        using E = std::pair<N, N>;
        using ED = _ED;
    public:
        E backtracking_edge;
        ED backtracking_weight;
    };

    template<typename _ED, typename _ND_CONTAINER, typename _ED_CONTAINER, typename T = unsigned int, bool error_check = true>
        requires std::is_floating_point_v<_ED> && std::is_integral_v<T> && std::is_unsigned_v<T>
    class pairwise_extended_gap_alignment_graph {
    public:
        using N = node<T>;
        using E = std::pair<N, N>;
        using ED = _ED;
        using ND = node_data<_ED, T>;

    private:
        offbynull::grid_graph::grid_graph_diagonal::grid_graph_diagonal<ND, _ED, _ND_CONTAINER, _ED_CONTAINER, T, error_check> g_grid;
        shifted_grid_graph_down<ND, _ND_CONTAINER, T, error_check> g_down;
        shifted_grid_graph_right<ND, _ND_CONTAINER, T, error_check> g_right;
        ED freeride_ed;  // data for all edges going from g_down/g_right to g_grid
        ED gap_ed;  // data for all edges in g_down and in g_right

    public:
        pairwise_extended_gap_alignment_graph(
            T _down_node_cnt,
            T _right_node_cnt,
            ED gap_score,
            std::function<_ND_CONTAINER(T, T)> nd_container_creator,
            std::function<_ED_CONTAINER(T, T)> ed_container_creator
        )
        : g_grid{_down_node_cnt, _right_node_cnt, nd_container_creator, ed_container_creator}
        , g_down{_down_node_cnt, _right_node_cnt, nd_container_creator, ed_container_creator}
        , g_right{_down_node_cnt, _right_node_cnt, nd_container_creator, ed_container_creator}
        , gap_ed{gap_score}
        , freeride_ed{0.0} {}

        void update_node_data(const N& node, ND&& data) {
            if constexpr (error_check) {
                if (!has_node(node)) {
                    throw std::runtime_error {"Node doesn't exist"};
                }
            }
            if (node.type == node_type::DIAGONAL) {
                g_grid.update_node_data(node.inner_node, std::forward<ND>(data));
            } else if (node.type == node_type::DOWN) {
                g_down.update_node_data(node.inner_node, std::forward<ND>(data));
            } else if (node.type == node_type::RIGHT) {
                g_right.update_node_data(node.inner_node, std::forward<ND>(data));
            }
        }

        ND& get_node_data(const N& node) {
            if (node.type == node_type::DIAGONAL) {
                return g_grid.get_node_data(node.inner_node);
            } else if (node.type == node_type::DOWN) {
                return g_down.get_node_data(node.inner_node);
            } else if (node.type == node_type::RIGHT) {
                return g_right.get_node_data(node.inner_node);
            }
        }

        void update_edge_data(const E& edge, ED&& data) {
            if constexpr (error_check) {
                if (!has_edge(edge)) {
                    throw std::runtime_error {"Edge doesn't exist"};
                }
            }
            const auto& [n1, n2] { edge.inner_edge };
            if (n1.type == node_type::DIAGONAL && n2.type == node_type::DIAGONAL) {  // match
                return g_grid.update_edge_data(edge.inner_edge, data);
            } else if (n1.type == node_type::DOWN && n2.type == node_type::DOWN) {  // gap
                gap_ed = std::forward<ED>(data);
            } else if (n1.type == node_type::RIGHT && n2.type == node_type::RIGHT) {  // gap
                gap_ed = std::forward<ED>(data);
            } else if (n1.type == node_type::DIAGONAL && n2.type == node_type::DOWN) {  // indel
                g_grid.update_edge_data(edge.inner_edge, data);
            } else if (n1.type == node_type::DIAGONAL && n2.type == node_type::RIGHT) {  // indel
                g_grid.update_edge_data(edge.inner_edge, data);
            } else if (n1.type == node_type::DOWN && n2.type == node_type::DIAGONAL) {  // freeride
                freeride_ed = std::forward<ED>(data);
            } else if (n1.type == node_type::RIGHT && n2.type == node_type::DIAGONAL) {  // freeride
                freeride_ed = std::forward<ED>(data);
            }
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
            const auto& [n1, n2] { edge.inner_edge };
            if (n1.type == node_type::DIAGONAL && n2.type == node_type::DIAGONAL) {  // match
                return g_grid.get_edge(edge.inner_edge);
            } else if (n1.type == node_type::DOWN && n2.type == node_type::DOWN) {  // gap
                return {n1, n2, gap_ed};
            } else if (n1.type == node_type::RIGHT && n2.type == node_type::RIGHT) {  // gap
                return {n1, n2, gap_ed};
            } else if (n1.type == node_type::DIAGONAL && n2.type == node_type::DOWN) {  // indel
                return g_grid.get_edge(edge.inner_edge);
            } else if (n1.type == node_type::DIAGONAL && n2.type == node_type::RIGHT) {  // indel
                return g_grid.get_edge(edge.inner_edge);
            } else if (n1.type == node_type::DOWN && n2.type == node_type::DIAGONAL) {  // freeride -- these are ghosted in
                return {n1, n2, freeride_ed};
            } else if (n1.type == node_type::RIGHT && n2.type == node_type::DIAGONAL) {  // freeride -- these are ghosted in
                return {n1, n2, freeride_ed};
            }
        }

        auto get_root_nodes() {
            return g_grid.get_root_nodes()
                | std::views::transform([&](const auto& n) noexcept { return N { node_type::DIAGONAL, n }; });
        }

        N get_root_node() {
            return N { node_type::DIAGONAL, n };
        }

        auto get_leaf_nodes() {
            return g_grid.get_leaf_nodes()
                | std::views::transform([&](const auto& n) noexcept { return N { node_type::DIAGONAL, n }; });
        }

        auto get_nodes() {
            auto diagonal_nodes {
                g_grid.get_nodes()
                | std::views::transform([&](const auto& n) noexcept { return N { node_type::DIAGONAL, n }; })
            };
            auto down_nodes {
                g_down.get_nodes()
                | std::views::transform([&](const auto& n) noexcept { return N { node_type::DOWN, n }; })
            };
            auto right_nodes {
                g_right.get_nodes()
                | std::views::transform([&](const auto& n) noexcept { return N { node_type::RIGHT, n }; })
            };
            // This should be using std::views::conat, but it wasn't included in this version of the C++ standard
            // library. The concat implementation below lacks several features (e.g. doesn't support the pipe operator)
            // and forcefully returns copies (concat_view::iterator::value_type ==
            // concat_view::iterator::reference_type).
            return concat_view(
                diagonal_nodes,
                concat_view(down_nodes, right_nodes)
            );
        }

        auto get_edges() {
            auto diagonal_edges {
                g_grid.get_edges()
                | std::views::transform([](const auto& e) noexcept -> E {
                    const auto& [n1, n2] { e };
                    if (n1 != n2) {  // This is always +1 down and +1 right
                        return E { {node_type::DIAGONAL, n1}, {node_type::DIAGONAL, n2} };
                    }
                    const auto& [n1_down, n1_right] { n1 };
                    const auto& [n2_down, n2_right] { n2 };
                    if (n1_right == n1_right) {  // This is always +1 down
                        return E { {node_type::DIAGONAL, n1}, {node_type::DOWN, n2} };
                    }
                    if (n1_down == n2_down) {  // This is always +1 right
                        return E { {node_type::DIAGONAL, n1}, {node_type::RIGHT, n2} };
                    }
                    // Should never reach this point
                })
            };
            auto down_edges {
                g_down.get_edges()
                | std::views::transform([&](const auto& e) noexcept -> E {
                    const auto& [n1, n2] { e };
                    return E { {node_type::DOWN, n1}, {node_type::DOWN, n2} };
                })
            };
            auto right_edges {
                g_right.get_edges()
                | std::views::transform([&](const auto& e) noexcept -> E {
                    const auto& [n1, n2] { e };
                    return E { {node_type::RIGHT, n1}, {node_type::RIGHT, n2} };
                })
            };
            auto down_to_diag_freeride_edges {
                std::views::cartesian_product(
                    std::views::iota(1u, g_grid.down_node_cnt),
                    std::views::iota(1u, g_grid.right_node_cnt)
                )
                | std::views::transform([&](const auto & e) noexcept -> E {
                    const auto& [n1, n2] { e };
                    return E { {node_type::DOWN, n1}, {node_type::DIAGONAL, n2} };
                })
            };
            auto right_to_diag_freeride_edges {
                std::views::cartesian_product(
                    std::views::iota(1u, g_grid.down_node_cnt),
                    std::views::iota(1u, g_grid.right_node_cnt)
                )
                | std::views::transform([&](const auto & e) noexcept -> E {
                    const auto& [n1, n2] { e };
                    return E { {node_type::DOWN, n1}, {node_type::RIGHT, n2} };
                })
            };
            // This should be using std::views::conat, but it wasn't included in this version of the C++ standard
            // library. The concat implementation below lacks several features (e.g. doesn't support the pipe operator)
            // and forcefully returns copies (concat_view::iterator::value_type ==
            // concat_view::iterator::reference_type).
            return concat_view {
                std::move(diagonal_edges),
                concat_view {
                    std::move(down_edges),
                    concat_view {
                        std::move(right_edges),
                        concat_view {
                            down_to_diag_freeride_edges,
                            right_to_diag_freeride_edges
                        }
                    }
                }
            };
        }

        bool has_node(const N& node) {
            if (node.type == node_type::DIAGONAL) {
                return g_grid.has_node(node.inner_node);
            } else if (node.type == node_type::DOWN) {
                return g_down.has_node(node.inner_node);
            } else if (node.type == node_type::RIGHT) {
                return g_right.has_node(node.inner_node);
            }
        }

        bool has_edge(const E& edge) {
            const auto& [n1, n2] { edge.inner_edge };
            if (n1.type == node_type::DIAGONAL && n2.type == node_type::DIAGONAL) {  // match
                return g_grid.has_edge(E {n1, n2 });
            } else if (n1.type == node_type::DOWN && n2.type == node_type::DOWN) {  // gap
                return g_down.has_edge(E { n1, n2 });
            } else if (n1.type == node_type::RIGHT && n2.type == node_type::RIGHT) {  // gap
                return g_right.has_edge(E { n1, n2 });
            } else if (n1.type == node_type::DIAGONAL && n2.type == node_type::DOWN) {  // indel
                return g_grid.has_edge(E {n1, n2});
            } else if (n1.type == node_type::DIAGONAL && n2.type == node_type::RIGHT) {  // indel
                return g_grid.has_edge(E {n1, n2});
            } else if (n1.type == node_type::DOWN && n2.type == node_type::DIAGONAL) {  // freeride -- these are ghosted in
                return n1 == n2;
            } else if (n1.type == node_type::RIGHT && n2.type == node_type::DIAGONAL) {  // freeride -- these are ghosted in
                return n1 == n2;
            }
            return false;
        }

        auto get_outputs_full(const N& node) {
            boost::container::static_vector<std::tuple<E, N, N, ED&>, 3> ret {};
            if (node.type == node_type::DIAGONAL) {
                const auto& r {
                    g_grid.get_outputs_full(node.inner_node)
                    | std::views::transform([](const auto& e_full) noexcept -> std::tuple<E, N, N, ED&> {
                        const auto& [e, n1, n2, ed] { e_full }; 
                        if (n1 != n2) {  // This is always +1 down and +1 right
							N new_n1 { node_type::DIAGONAL, n1 };
							N new_n2 { node_type::DIAGONAL, n2 };
							E new_e { new_n1, new_n2 };
                            return { new_e, new_n1, new_n2, ed }; 
                        }
                        if (n1_right == n1_right) {  // This is always +1 down
							N new_n1 { node_type::DIAGONAL, n1 };
							N new_n2 { node_type::DOWN, n2 };
							E new_e { new_n1, new_n2 };
                            return { new_e, new_n1, new_n2, ed };
                        }
                        if (n1_down == n2_down) {  // This is always +1 right
							N new_n1 { node_type::DIAGONAL, n1 };
							N new_n2 { node_type::RIGHT, n2 };
							E new_e { new_n1, new_n2 };
                            return { new_e, new_n1, new_n2, ed };
                        }
                        // Should never reach this point
                    })
                };
                std::ranges::copy(r, std::back_inserter(ret));
            } else if (node.type == node_type::DOWN) {
                const auto& r {
                    g_down.get_outputs_full(node)
                    | std::views::transform([](const auto& e_full) noexcept -> std::tuple<E, N, N, ED&> {
						const auto& [e, n1, n2, ed] { e_full };
						N new_n1 { node_type::DOWN, n1 };
						N new_n2 { node_type::DOWN, n2 };
						E new_e { new_n1, new_n2 };
                        return { new_e, new_n1, new_n2, ed }; 
                    })
                };
                std::ranges::copy(r, std::back_inserter(ret));
				// add free ride
				N new_n1 { node_type::DOWN, node };
				N new_n2 { node_type::DIAGONAL, node };
				E new_e { new_n1, new_n2 };
                ret.push_back({ new_e, new_n1, new_n2, freeride_ed);
            } else if (node.type == node_type::RIGHT) {
                const auto& r {
                    g_right.get_outputs_full(node)
                    | std::views::transform([](const auto& e_full) noexcept -> std::tuple<E, N, N, ED&> {
						const auto& [e, n1, n2, ed] { e_full };
						N new_n1 { node_type::RIGHT, n1 };
						N new_n2 { node_type::RIGHT, n2 };
						E new_e { new_n1, new_n2 };
                        return { new_e, new_n1, new_n2, ed };
                    })
                };
                std::ranges::copy(r, std::back_inserter(ret));
				// add free ride
				N new_n1 { node_type::RIGHT, node };
				N new_n2 { node_type::DIAGONAL, node };
				E new_e { new_n1, new_n2 };
                ret.push_back({ new_e, new_n1, new_n2, freeride_ed);
            }
            return ret;
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
            boost::container::static_vector<std::tuple<E, N, N, ED&>, 5> ret {};
            if (node.type == node_type::DIAGONAL) {
                const auto& r {
                    g_grid.get_outputs_full(node.inner_node)
                    | std::views::transform([](const auto& e_full) noexcept -> std::tuple<E, N, N, ED&> {
                        const auto& [e, n1, n2, ed] { e_full }; 
                        if (n1 != n2) {  // This is always +1 down and +1 right
							N new_n1 { node_type::DIAGONAL, n1 };
							N new_n2 { node_type::DIAGONAL, n2 };
							E new_e { new_n1, new_n2 };
                            return { new_e, new_n1, new_n2, ed }; 
                        }
                        if (n1_right == n1_right) {  // This is always +1 down
							N new_n1 { node_type::DIAGONAL, n1 };
							N new_n2 { node_type::DOWN, n2 };
							E new_e { new_n1, new_n2 };
                            return { new_e, new_n1, new_n2, ed };
                        }
                        if (n1_down == n2_down) {  // This is always +1 right
							N new_n1 { node_type::DIAGONAL, n1 };
							N new_n2 { node_type::RIGHT, n2 };
							E new_e { new_n1, new_n2 };
                            return { new_e, new_n1, new_n2, ed };
                        }
                        // Should never reach this point
                    })
                };
                std::ranges::copy(r, std::back_inserter(ret));
				// add free ride
				{
					N new_n1 { node_type::DOWN, node };
					N new_n2 { node_type::DIAGONAL, node };
					E new_e { new_n1, new_n2 };
            	    ret.push_back({ new_e, new_n1, new_n2, freeride_ed);
				}
				// add free ride
				{				
					N new_n1 { node_type::RIGHT, node };
					N new_n2 { node_type::DIAGONAL, node };
					E new_e { new_n1, new_n2 };
                	ret.push_back({ new_e, new_n1, new_n2, freeride_ed);
				}
            } else if (node.type == node_type::DOWN) {
                const auto& r {
                    g_down.get_outputs_full(node)
                    | std::views::transform([](const auto& e_full) noexcept -> std::tuple<E, N, N, ED&> {
						const auto& [e, n1, n2, ed] { e_full };
						N new_n1 { node_type::DOWN, n1 };
						N new_n2 { node_type::DOWN, n2 };
						E new_e { new_n1, new_n2 };
                        return { new_e, new_n1, new_n2, ed }; 
                    })
                };
                std::ranges::copy(r, std::back_inserter(ret));
            } else if (node.type == node_type::RIGHT) {
                const auto& r {
                    g_right.get_outputs_full(node)
                    | std::views::transform([](const auto& e_full) noexcept -> std::tuple<E, N, N, ED&> {
						const auto& [e, n1, n2, ed] { e_full };
						N new_n1 { node_type::RIGHT, n1 };
						N new_n2 { node_type::RIGHT, n2 };
						E new_e { new_n1, new_n2 };
                        return { new_e, new_n1, new_n2, ed };
                    })
                };
                std::ranges::copy(r, std::back_inserter(ret));
            }
            return ret;
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
        return pairwise_extended_gap_alignment_graph<_ED, std::vector<ND>, std::vector<_ED>, T, error_check> {
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
        return pairwise_extended_gap_alignment_graph<_ED, std::array<ND, size>, std::array<_ED, size>, T, error_check> {
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
        return pairwise_extended_gap_alignment_graph<_ED, boost::container::small_vector<ND, stack_size>, boost::container::small_vector<_ED, stack_size>, T, error_check> {
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
        return pairwise_extended_gap_alignment_graph<_ED, boost::container::static_vector<ND, stack_size>, boost::container::static_vector<_ED, stack_size>, T, error_check> {
            down_cnt,
            right_cnt,
            [actual_size](T, T) { return boost::container::static_vector<ND, stack_size>(actual_size); },
            [actual_size](T, T) { return boost::container::static_vector<offbynull::grid_graph::grid_graph::edge_data_set<_ED>, stack_size>(actual_size); }
        };
    }
}
#endif //PAIRWISE_EXTENDED_GAP_ALIGNMENT_GRAPH_H
