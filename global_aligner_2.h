#ifndef GLOBAL_ALIGNER_2_H
#define GLOBAL_ALIGNER_2_H

#include <optional>
#include <tuple>
#include <vector>
#include <functional>
#include <utility>
#include "boost/optional.hpp"
#include "backtracker.h"
#include "grid_graph_2.h"

template<typename _ELEM>
class global_aligner {
public:
    using N = std::pair<unsigned int, unsigned int>;
    using E = std::pair<N, N>;

private:
    class node_data {
        friend class global_aligner;
    private:
        boost::optional<double> weight;
        boost::optional<E> backtracking_edge;

    public:
        node_data() : weight{boost::none}, backtracking_edge{boost::none} {}

        void set_weight_and_backtracking_edge(double weight, const E& backtracking_edge) {
            *this->weight = weight;
            *this->backtracking_edge = backtracking_edge;
        }

        std::tuple<boost::optional<double>, boost::optional<E>> get_weight_and_backtracking_edge() {
            return {weight, backtracking_edge};
        }
    };

    class edge_data {
        friend class global_aligner;
    private:
        boost::optional<_ELEM> v_elem;
        boost::optional<_ELEM> w_elem;
        double weight;

    public:
        edge_data(boost::optional<_ELEM> v_elem = boost::none, boost::optional<_ELEM> w_elem = boost::none, double weight = 0.0)
            : v_elem{v_elem}
            , w_elem{w_elem}
            , weight{weight} {}

        std::tuple<boost::optional<_ELEM>, boost::optional<_ELEM>> get_elements() {
            return { this->v_elem, this->w_elem };
        }
    };

    template <typename _ELEM_RET>
    std::tuple<double, std::vector<std::pair<boost::optional<_ELEM_RET>, boost::optional<_ELEM_RET>>>> align_raw(
            const std::vector<_ELEM>& v,
            const std::vector<_ELEM>& w,
            std::function<double(const boost::optional<_ELEM_RET>&, const boost::optional<_ELEM_RET>&)> weight_lookup
    ) {
        auto v_node_cnt { v.size() + 1u };
        auto w_node_cnt { w.size() + 1u };
        grid_graph<node_data, edge_data> g { v_node_cnt, w_node_cnt };
        for (const auto& e : g.get_edges()) {
            const auto& [n1, n2] { e };
            const auto& [n1_down, n1_right] { n1 };
            const auto& [n2_down, n2_right] { n2 };
            edge_data& ed { g.get_edge_data(e) };
            if (n1_down == n2_down && n1_right + 1u == n2_right) {
                ed.weight = weight_lookup(std::nullopt, { w[n1_right] });
                ed.w_elem = { w[n1_right] };
            } else if (n1_down + 1u == n2_down && n1_right == n2_right) {
                ed.weight = weight_lookup({ v[n1_down] }, std::nullopt);
                ed.v_elem = { v[n1_down] };
            } else if (n1_down + 1u == n2_down && n1_right + 1u == n2_right) {
                ed.weight = weight_lookup({ v[n1_down] }, { w[n1_right] });
                ed.v_elem = { v[n1_down] };
                ed.w_elem = { w[n1_right] };
            } else {
                throw std::runtime_error("This should never happen");
            }
        }
        N from_node { 0u, 0u };
        N to_node { v_node_cnt - 1u, w_node_cnt - 1u};
        backtracker<decltype(g)> b{};
        auto [path, weight] = *b.find_max_path(
            g,
            from_node,
            to_node,
            [&g](N node, std::optional<double> weight, std::optional<E> backtracking_edge) {
                node_data& nd { g.get_node_data(node) };
                nd.weight = weight;
                nd.backtracking_edge = backtracking_edge;
            },
            [&g](N node) {
                node_data& nd { g.get_node_data(node) };
                return std::tuple<boost::optional<double>, boost::optional<E>> { nd.weight, nd.backtracking_edge };
            },
            [&g](E edge) {
                edge_data& nd { g.get_edge_data(edge) };
                return g.get_edge_data(edge).weight;
            }
        );
        std::vector<std::pair<boost::optional<_ELEM_RET>, boost::optional<_ELEM_RET>>> alignment {};
        for (const auto& e : path) {
            const auto& ed = g.get_edge_data(e);
            alignment.push_back({ { ed.v_elem.get() }, { ed.w_elem.get() } });
        }
        return { weight, alignment };
    }

public:
    std::tuple<double, std::vector<std::pair<boost::optional<_ELEM>, boost::optional<_ELEM>>>> align_copy(
            const std::vector<_ELEM>& v,
            const std::vector<_ELEM>& w,
            std::function<double(const boost::optional<_ELEM>&, const boost::optional<_ELEM>&)> weight_lookup
    ) {
        return align_raw(v, w, weight_lookup);
    }

    auto align_ref(
            const std::vector<_ELEM>& v,
            const std::vector<_ELEM>& w,
            std::function<double(const boost::optional<const _ELEM &>&, const boost::optional<const _ELEM &>&)> weight_lookup
    ) {
        return align_raw<const _ELEM &>(v, w, weight_lookup);
    }

    auto align_ref(
            const std::vector<_ELEM>&& v,
            const std::vector<_ELEM>&& w,
            std::function<double(const boost::optional<const _ELEM &>&, const boost::optional<const _ELEM &>&)> weight_lookup
    ) = delete;

    auto align_ref(
        const std::vector<_ELEM>& v,
        const std::vector<_ELEM>&& w,
        std::function<double(const boost::optional<const _ELEM &>&, const boost::optional<const _ELEM &>&)> weight_lookup
    ) = delete;

    auto align_ref(
        const std::vector<_ELEM>&& v,
        const std::vector<_ELEM>& w,
        std::function<double(const boost::optional<const _ELEM &>&, const boost::optional<const _ELEM &>&)> weight_lookup
    ) = delete;

    auto align(
            const std::vector<_ELEM>& v,
            const std::vector<_ELEM>& w,
            const auto& weight_lookup
    ) {
        if constexpr (sizeof(const _ELEM &) > sizeof(_ELEM)) {
            return align_copy(v, w, weight_lookup);
        } else {
            return align_ref(v, w, weight_lookup);
        }
    }
};

#endif //GLOBAL_ALIGNER_2_H
