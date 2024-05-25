#ifndef OFFBYNULL_ALIGNER_ALIGNER_H
#define OFFBYNULL_ALIGNER_ALIGNER_H

#include <iostream>
#include <functional>
#include <utility>
#include <stdfloat>
#include <cstdint>
#include "offbynull/aligner/backtrack/backtrack.h"
#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"
#include "offbynull/aligner/graph/utils.h"

namespace offbynull::aligner {
    using offbynull::aligner::backtrack::backtrack::find_max_path;
    using offbynull::aligner::backtrack::slot_container::slot;
    using offbynull::aligner::graph::graph::readable_graph;
    using offbynull::aligner::graph::grid_container_creator::grid_container_creator;
    using offbynull::aligner::graph::grid_container_creators::vector_grid_container_creator;
    using offbynull::aligner::graph::grid_container_creators::array_grid_container_creator;
    using offbynull::aligner::graphs::pairwise_global_alignment_graph::pairwise_global_alignment_graph;
    using offbynull::aligner::backtrack::container_creator::container_creator;
    using offbynull::aligner::backtrack::container_creators::vector_container_creator;
    using offbynull::aligner::backtrack::container_creators::array_container_creator;
    using offbynull::aligner::backtrack::container_creators::static_vector_container_creator;

    using ND = std::tuple<>;

    template <readable_graph G, container_creator SLOT_ALLOCATOR, container_creator PATH_ALLOCATOR, bool error_check>
    auto align(
        readable_graph auto& graph,
        std::ranges::range auto& v,
        std::ranges::range auto& w
    ) {
        using E = typename G::E;
        auto [path, weight] {
            find_max_path<G, SLOT_ALLOCATOR, PATH_ALLOCATOR, error_check>(
                graph,
                graph.get_root_node(),
                *graph.get_leaf_nodes().begin(),
                [&](const E& e) { return graph.get_edge_data(e); }
            )
        };
        return std::make_pair(
            std::move(path)
                | std::views::transform([&](const auto& edge) { return graph.edge_to_elements(edge, v, w); })
                | std::views::filter([](const auto& elem_pair) { return elem_pair.has_value(); })
                | std::views::transform([](const auto& elem_pair) { return *elem_pair; }),
            weight
        );
    }
}

#endif //OFFBYNULL_ALIGNER_ALIGNER_H
