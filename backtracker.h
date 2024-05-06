#ifndef BACKTRACKER_H
#define BACKTRACKER_H

#include <tuple>
#include <optional>
#include <vector>
#include <set>
#include <map>
#include <functional>
#include <ranges>
#include <algorithm>

template<typename G>
class backtracker {
public:
    using N = typename G::N;
    using ND = typename G::ND;
    using E = typename G::E;
    using ED = typename G::ED;

    void populate_weights_and_backtrack_pointers(
            G& g,
            const N& from_node,
            std::function<void(N, std::optional<double>, std::optional<E>)> set_node_data_func,
            std::function<std::tuple<std::optional<double>, std::optional<E>>(N)> get_node_data_func,
            std::function<double(E)> get_edge_weight_func
    ) {
        std::set<N> processed_nodes {}; // nodes where all parents have been processed AND it has been processed
        std::set<N> waiting_nodes {};   // nodes where all parents have been processed BUT it has yet to be processed
        std::map<N, size_t> unprocessable_nodes {};   // nodes that have some parents remaining to be processed (value=# of parents left)
        // For all root nodes, add to processed_nodes and set None weight and None backtracking edge.
        for (const auto& node : g.get_root_nodes()) {
            set_node_data_func(node, std::nullopt, std::nullopt);
            processed_nodes.insert(node);
        }
        // For all root nodes, add any children where its the only parent to waiting_nodes.
        for (const auto& node : processed_nodes) {
            for (const auto e : g.get_outputs(node)) {
                const auto dst_node = g.get_edge_to(e);
                auto e_from_set = g.get_inputs(dst_node) | std::views::transform([&g](auto _e) { return g.get_edge_from(_e); });
                // NOTE: std::includes is safe to use because set::set<> iterates over keys in sorted order?
                bool is_subset = std::includes(e_from_set.begin(), e_from_set.end(), processed_nodes.begin(), processed_nodes.end());
                if (is_subset) {
                    waiting_nodes.insert(dst_node);
                }
            }
        }
        // Make sure from_node is a root and set its weight to 0.
        if (!processed_nodes.contains(from_node)) {
            throw std::runtime_error("Not a root node");
        }
        set_node_data_func(from_node, 0.0, std::nullopt);
        // Track how many remaining parents each node in the graph has. Note that the graph's root nodes were already marked
        // as processed above.
        for (const auto node : g.get_nodes()) {
            std::set<N> incoming_nodes {};
            for (const auto e : g.get_inputs(node)) {
                incoming_nodes.insert(g.get_edge_from(e));
            }
            auto unprocessed_cnt = std::ranges::distance(
                incoming_nodes
                | std::views::filter([&processed_nodes](const auto n) { return !processed_nodes.contains(n); })
            );
            unprocessable_nodes[node] = unprocessed_cnt;
        }
        // Any nodes in waiting_nodes have had all their parents already processed (in processed_nodes). As such, they can
        // have their weights and backtracking pointers calculated. They can then be placed into processed_nodes themselves.
        while (waiting_nodes.size() > 0u) {
            const auto node = *waiting_nodes.begin();
            std::set<N> incoming_nodes {};
            for (const auto e : g.get_inputs(node)) {
                incoming_nodes.insert(g.get_edge_from(e));
            }
            bool is_subset = std::includes(processed_nodes.begin(), processed_nodes.end(), incoming_nodes.begin(), incoming_nodes.end());
            if (!is_subset) {
                continue;
            }
            std::map<E, double> incoming_accum_weights {};
            for (const auto edge : g.get_inputs(node)) {
                const auto src_node = g.get_edge_from(edge);
                const auto [src_node_weight, _] = get_node_data_func(src_node);
                const auto edge_weight = get_edge_weight_func(edge);
                // Roots that aren't from_node were initialized to a weight of None -- if you see them, skip them.
                if (src_node_weight != std::nullopt) {
                    incoming_accum_weights[edge] = *src_node_weight + edge_weight;
                }
            }
            std::optional<double> max_weight = std::nullopt;
            std::optional<E> max_edge = std::nullopt;
            if (!incoming_accum_weights.empty()) {
                auto found = std::ranges::max_element(
                    incoming_accum_weights,
                    [](const auto& a, const auto& b) noexcept {
                        return a.second < b.second;
                    }
                );
                max_edge = found->first;
                max_weight = found->second;
            }
            set_node_data_func(node, max_weight, max_edge);
            // This node has been processed, move it over to processed_nodes.
            waiting_nodes.erase(node);
            processed_nodes.insert(node);
            // For outgoing nodes this node points to, if that outgoing node has all of its dependencies in processed_nodes,
            // then add it to waiting_nodes (so it can be processed).
            const auto outgoing_nodes {g.get_outputs(node) | std::views::transform([&g](const auto e) { return g.get_edge_to(e); })};
            for (const auto output_node : outgoing_nodes) {
                unprocessable_nodes[output_node] -= 1u;
                if (unprocessable_nodes[output_node] == 0u) {
                    waiting_nodes.insert(output_node);
                }
            }
        }
    }

    std::vector<E> backtrack(
            G& g,
            const N& end_node,
            std::function<std::tuple<std::optional<double>, std::optional<E>>(N)> get_node_data_func
    ) {
        auto next_node { end_node };
        std::vector<E> reverse_path {};
        while (true) {
            auto node { next_node };
            auto [weight, backtracking_edge] { get_node_data_func(node) };
            if (backtracking_edge == std::nullopt) {
                break;
            }
            reverse_path.push_back(*backtracking_edge);
            next_node = g.get_edge_from(*backtracking_edge);
        }
        std::reverse(reverse_path.begin(), reverse_path.end());  // this is the path in reverse -- reverse it to get it in the correct order
        return reverse_path;
    }

    std::optional<std::tuple<std::vector<E>, double>> find_max_path(
            G& graph,
            const N& start_node,
            const N& end_node,
            std::function<void(N, std::optional<double>, std::optional<E>)> set_node_data_func,
            std::function<std::tuple<std::optional<double>, std::optional<E>>(N)> get_node_data_func,
            std::function<double(E)> get_edge_weight_func
    ) {
        populate_weights_and_backtrack_pointers(
            graph,
            start_node,
            set_node_data_func,
            get_node_data_func,
            get_edge_weight_func
        );
        auto path = backtrack(graph, end_node, get_node_data_func);
        if (path.empty()) {
            return std::nullopt;
        }
        auto [weight, _] = get_node_data_func(end_node);
        return std::tuple<std::vector<E>, double>{path, *weight};
    }
};

#endif //BACKTRACKER_H
