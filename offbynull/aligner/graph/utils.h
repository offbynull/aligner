#ifndef OFFBYNULL_ALIGNER_GRAPH_UTILS_H
#define OFFBYNULL_ALIGNER_GRAPH_UTILS_H

#include <format>
#include <string>
#include "offbynull/aligner/graph/graph.h"

namespace offbynull::aligner::graph::utils {
    using offbynull::aligner::graph::graph::readable_graph;

    std::string graph_to_string(readable_graph auto &g) {
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

#endif //OFFBYNULL_ALIGNER_GRAPH_UTILS_H
