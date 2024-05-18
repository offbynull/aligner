#ifndef GRAPH_HELPERS_H
#define GRAPH_HELPERS_H

#include <format>
#include <string>

namespace offbynull::graph::graph_helpers {
    std::string graph_to_string(const auto &g) {
        std::string out {};
        for (const auto& node : g.get_nodes()) {
            out += std::format("node {}: {}\n", node, g.get_node_data(node));
            for (const auto& edge : g.get_outputs(node)) {
                auto [from_node, to_node, edge_data] = g.get_edge(edge);
                out += std::format("  edge {} pointing to node {}: {}\n", edge, to_node, edge_data);
            }
        }
        return out;
    }
}

#endif //GRAPH_HELPERS_H
