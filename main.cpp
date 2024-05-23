#include <iostream>
#include <functional>
#include <offbynull/aligner/backtrack/backtrack.h>
#include <offbynull/aligner/graphs/pairwise_extended_gap_alignment_graph.h>

#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"
#include "offbynull/aligner/graph/utils.h"

auto global(
    std::ranges::range auto&& v,
    std::ranges::range auto&& w,
    std::function<
        double(
            const std::optional<std::reference_wrapper<const std::remove_reference_t<decltype(v[0u])>>>&,
            const std::optional<std::reference_wrapper<const std::remove_reference_t<decltype(w[0u])>>>&
        )
    > weight_lookup
) {
    static_assert(!std::is_rvalue_reference_v<decltype(v)>, "v cannot be an rvalue reference: Function returns references into v, meaning v should continue to exist once function returns.");
    static_assert(!std::is_rvalue_reference_v<decltype(w)>, "w cannot be an rvalue reference: Function returns references into w, meaning w should continue to exist once function returns.");
    using offbynull::aligner::graphs::pairwise_global_alignment_graph::pairwise_global_alignment_graph;
    using offbynull::aligner::backtrack::backtrack::find_max_path;

    pairwise_global_alignment_graph<std::tuple<>, double, size_t> graph { v.size() + 1u, w.size() + 1u };
    // using N = decltype(graph)::N;
    using E = decltype(graph)::E;
    graph.assign_weights<double>(
        v,
        w,
        weight_lookup,
        [](double& edge_data, double weight) { edge_data = weight; }
    );
    auto [path, weight] {
        find_max_path(
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

int main() {
    std::string v { "hello" };
    std::string w { "mellow" };
    auto [elements, weight] {
        global(
            v,
            w,
            [](const auto& v_elem, const auto& w_elem) -> double {
                if (!v_elem.has_value() || !w_elem.has_value()) {
                    return 0.0;
                }
                return v_elem.value().get() == w_elem.value().get() ? 1.0 : 0.0;
            }
        )
    };
    std::string v_align {};
    std::string w_align {};
    for (const auto& [v_elem_opt, w_elem_opt] : elements) {
        v_align += (v_elem_opt.has_value() ? *v_elem_opt : '-');
        w_align += (w_elem_opt.has_value() ? *w_elem_opt : '-');
    }
    std::cout << v_align << std::endl;
    std::cout << w_align << std::endl;
    return 0;
    // using offbynull::aligner::graph::utils::graph_to_string;
    // using offbynull::aligner::graphs::directed_graph::directed_graph;
    //
    // directed_graph<std::string, std::string, std::string, std::string> g {};
    //
    // g.insert_node("A", "");
    // g.insert_node("B", "");
    // g.insert_node("C", "");
    // g.insert_node("D", "");
    // g.insert_edge("AB1", "A", "B", "MY EDGE DATA FOR 1st AB");
    // g.insert_edge("AB2", "A", "B", "MY EDGE DATA FOR 2nd AB");
    // g.insert_edge("BC", "B", "C", "");
    // g.insert_edge("CD", "C", "D", "");
    // g.insert_edge("AD", "A", "D", "");
    // std::cout << std::format("{} {}", g.get_in_degree("A"), g.get_out_degree("A")) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("B"), g.get_out_degree("B")) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("C"), g.get_out_degree("C")) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("D"), g.get_out_degree("D")) << std::endl;
    // g.insert_node("E", "");
    // g.insert_edge("DE", "D", "E", "");
    // g.delete_edge("AD");
    // // g.delete_edge("AC");  // error expected here
    //
    // std::cout << graph_to_string(g) << std::endl;
    // return 0;
}