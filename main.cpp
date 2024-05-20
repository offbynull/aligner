#include "offbynull/aligner/graph/graph_helpers.h"
#include "offbynull/aligner/graphs/directed_graph.h"

int main() {
    using G = offbynull::aligner::graph::graph_helpers::VectorAllocator<std::string, unsigned int>;
    static_assert(offbynull::aligner::graph::graph_helpers::allocator<G>);
    return 0;
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
    // std::cout << std::format("{}", g.to_string()) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("A"), g.get_out_degree("A")) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("B"), g.get_out_degree("B")) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("C"), g.get_out_degree("C")) << std::endl;
    // std::cout << std::format("{} {}", g.get_in_degree("D"), g.get_out_degree("D")) << std::endl;
    // g.insert_node("E", "");
    // g.insert_edge("DE", "D", "E", "");
    // std::cout << std::format("{}", g.to_string()) << std::endl;
    // g.delete_edge("AD");
    // std::cout << std::format("{}", g.to_string()) << std::endl;
    // g.delete_edge("AC");  // error expected here
    // std::cout << std::format("{}", g.to_string()) << std::endl;
}