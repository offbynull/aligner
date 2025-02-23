#ifndef OFFBYNULL_ALIGNER_GRAPH_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_H
#define OFFBYNULL_ALIGNER_GRAPH_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_H

#include <cstddef>
#include <concepts>
#include <ranges>
#include <tuple>
#include <stdexcept>
#include "offbynull/concepts.h"
#include "offbynull/helpers/join_bidirectional_view.h"
#include "offbynull/aligner/graph/pairwise_alignment_graph.h"

namespace offbynull::aligner::graph::sliceable_pairwise_alignment_graph {
    using offbynull::concepts::range_of_one_of;
    using offbynull::concepts::unqualified_object_type;
    using offbynull::helpers::join_bidirectional_view::join_bidirectional_view_adaptor;
    using offbynull::aligner::graph::pairwise_alignment_graph::readable_pairwise_alignment_graph;

    template <typename G>
    concept readable_sliceable_pairwise_alignment_graph =
        unqualified_object_type<G>
        && readable_pairwise_alignment_graph<G>
        && requires(
            G g,
            typename G::N node,
            typename G::INDEX grid_down,
            typename G::INDEX grid_right
        ) {
            { g.resident_nodes_capacity } -> std::same_as<const std::size_t&>;
            { g.row_nodes(grid_down) } -> range_of_one_of<typename G::N, const typename G::N&>;
            { g.row_nodes(grid_down, node, node) } -> range_of_one_of<typename G::N, const typename G::N&>;
            { g.is_reachable(node, node) } -> std::same_as<bool>;
            { g.resident_nodes() } -> range_of_one_of<typename G::N, const typename G::N&>;
            { g.outputs_to_residents(node) } -> range_of_one_of<typename G::E, const typename G::E&>;  // children in slice
            { g.inputs_from_residents(node) } -> range_of_one_of<typename G::E, const typename G::E&>;  // children in slice
        };




    // Reference implementation for row_nodes()
    // ----------------------------------------
    // You can either ...
    // 1. have the readable_sliceable_pairwise_alignment_graph's row_nodes() function call into this function
    // 2. write a custom implementation for the readable_sliceable_pairwise_alignment_graph's row_nodes() (typically more optimized than)
    //    and test against this to ensure things are working correctly.
    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G
    >
    std::ranges::bidirectional_range auto generic_row_nodes(
        const G& g,
        typename G::INDEX grid_down,
        const typename G::N& root_node,
        const typename G::N& leaf_node
    ) {
        using INDEX = typename G::INDEX;
        using N = typename G::N;

        if constexpr (debug_mode) {
            if (!g.has_node(root_node) || !g.has_node(leaf_node)) {
                throw std::runtime_error { "Bad root / leaf node " };
            }
        }

        INDEX right_first { std::get<1>(g.node_to_grid_offset(root_node)) };
        INDEX right_last { std::get<1>(g.node_to_grid_offset(leaf_node)) };
        return
            std::views::iota(right_first, right_last + 1u)
            | std::views::transform([&g, grid_down](const auto& grid_right) {
                return g.grid_offset_to_nodes(grid_down, grid_right);
            })
            | join_bidirectional_view_adaptor {}
            | std::views::filter([&g, root_node, leaf_node](const N& node) {
                return g.is_reachable(root_node, node) && g.is_reachable(node, leaf_node);
            });
    }

    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G
    >
    std::ranges::bidirectional_range auto generic_row_nodes(
        const G& g,
        typename G::INDEX grid_down
    ) {
        return generic_row_nodes<debug_mode, G>(g, grid_down, g.get_root_node(), g.get_leaf_node());
    }
}

#endif //OFFBYNULL_ALIGNER_GRAPH_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_H
