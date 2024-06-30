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
                const auto& [from_node, to_node, edge_data] { g.get_edge(edge) };
                out += std::format("  edge {} pointing to node {}: {}\n", edge, to_node, edge_data);
            }
        }
        return out;
    }

    struct generic_slicable_pairwise_alignment_graph_limits {
        const std::size_t max_grid_node_cnt;
        const std::size_t max_grid_node_depth;
        const std::size_t max_path_edge_cnt;
        const std::size_t max_slice_nodes_cnt;
        const std::size_t max_resident_nodes_cnt;

        constexpr generic_slicable_pairwise_alignment_graph_limits(
            std::size_t max_grid_node_cnt_,
            std::size_t max_grid_node_depth_,
            std::size_t max_path_edge_cnt_,
            std::size_t max_slice_nodes_cnt_,
            std::size_t max_resident_nodes_cnt_
        )
        : max_grid_node_cnt{ max_grid_node_cnt_ }
        , max_grid_node_depth{ max_grid_node_depth_ }
        , max_path_edge_cnt{ max_path_edge_cnt_ }
        , max_slice_nodes_cnt{ max_slice_nodes_cnt_ }
        , max_resident_nodes_cnt{ max_resident_nodes_cnt_ } {}
    };
}

#endif //OFFBYNULL_ALIGNER_GRAPH_UTILS_H
