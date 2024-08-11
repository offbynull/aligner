#include <format>
#include <string>
#include <iostream>
#include "offbynull/aligner/graphs/directed_graph.h"
#include "offbynull/aligner/graph/utils.h"
#include "gtest/gtest.h"

namespace {
    TEST(UtilsTest, GraphToString) {
        using offbynull::aligner::graph::utils::graph_to_string;
        using offbynull::aligner::graphs::directed_graph::directed_graph;

        directed_graph<true, std::string, std::string, std::string, std::string> g {};

        g.insert_node("A", "");
        g.insert_node("B", "");
        g.insert_node("C", "");
        g.insert_node("D", "");
        g.insert_edge("AB1", "A", "B", "MY EDGE DATA FOR 1st AB");
        g.insert_edge("AB2", "A", "B", "MY EDGE DATA FOR 2nd AB");
        g.insert_edge("BC", "B", "C", "");
        g.insert_edge("CD", "C", "D", "");
        g.insert_edge("AD", "A", "D", "");
        g.insert_node("E", "");
        g.insert_edge("DE", "D", "E", "");
        g.delete_edge("AD");

        EXPECT_FALSE(graph_to_string(g).empty());
    }
}