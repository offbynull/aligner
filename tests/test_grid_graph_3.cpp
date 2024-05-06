#include "grid_graph_3.h"
#include "gtest/gtest.h"
#include <iostream>

namespace {
    TEST(GridGraphTest3, ListNodes) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        auto n = g.get_nodes();
        EXPECT_EQ(
            std::set(n.begin(), n.end()),
            (std::set {
                std::pair{0u, 0u}, std::pair{0u, 1u}, std::pair{0u, 2u},
                std::pair{1u, 0u}, std::pair{1u, 1u}, std::pair{1u, 2u}
            })
        );
    }

    TEST(GridGraphTest3, ListEdges) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        auto e = g.get_edges();
        std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
        for (auto _e : e) {
            actual.insert(_e);
        }
        EXPECT_EQ(
            actual,
            (std::set<decltype(g)::E> {
                std::pair { std::pair{0u, 0u}, std::pair{0u, 1u} },
                std::pair { std::pair{0u, 1u}, std::pair{0u, 2u} },
                std::pair { std::pair{1u, 0u}, std::pair{1u, 1u} },
                std::pair { std::pair{1u, 1u}, std::pair{1u, 2u} },
                std::pair { std::pair{0u, 0u}, std::pair{1u, 0u} },
                std::pair { std::pair{0u, 1u}, std::pair{1u, 1u} },
                std::pair { std::pair{0u, 2u}, std::pair{1u, 2u} },
                std::pair { std::pair{0u, 0u}, std::pair{1u, 1u} },
                std::pair { std::pair{0u, 1u}, std::pair{1u, 2u} }
            })
        );
    }

    TEST(GridGraphTest3, NodesExist) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        EXPECT_TRUE(g.has_node({0u, 0u}));
        EXPECT_TRUE(g.has_node({0u, 1u}));
        EXPECT_TRUE(g.has_node({0u, 2u}));
        EXPECT_FALSE(g.has_node({0u, 3u}));
        EXPECT_TRUE(g.has_node({1u, 0u}));
        EXPECT_TRUE(g.has_node({1u, 1u}));
        EXPECT_TRUE(g.has_node({1u, 2u}));
        EXPECT_FALSE(g.has_node({1u, 3u}));
        EXPECT_FALSE(g.has_node({2u, 3u}));
    }

    TEST(GridGraphTest3, RightEdgesExist) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        EXPECT_TRUE(g.has_edge({{0u, 0u}, {0u, 1u}}));
        EXPECT_TRUE(g.has_edge({{0u, 1u}, {0u, 2u}}));
        EXPECT_FALSE(g.has_edge({{0u, 2u}, {0u, 3u}}));
        EXPECT_TRUE(g.has_edge({{1u, 0u}, {1u, 1u}}));
        EXPECT_TRUE(g.has_edge({{1u, 1u}, {1u, 2u}}));
        EXPECT_FALSE(g.has_edge({{1u, 2u}, {1u, 3u}}));
    }

    TEST(GridGraphTest3, DownEdgesExist) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        EXPECT_TRUE(g.has_edge({{0u, 0u}, {1u, 0u}}));
        EXPECT_FALSE(g.has_edge({{1u, 0u}, {2u, 0u}}));
        EXPECT_TRUE(g.has_edge({{0u, 1u}, {1u, 1u}}));
        EXPECT_FALSE(g.has_edge({{1u, 1u}, {2u, 1u}}));
        EXPECT_TRUE(g.has_edge({{0u, 2u}, {1u, 2u}}));
        EXPECT_FALSE(g.has_edge({{1u, 2u}, {2u, 2u}}));
    }

    TEST(GridGraphTest3, DiagEdgesExist) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        EXPECT_TRUE(g.has_edge({{0u, 0u}, {1u, 1u}}));
        EXPECT_FALSE(g.has_edge({{1u, 0u}, {2u, 1u}}));
        EXPECT_TRUE(g.has_edge({{0u, 1u}, {1u, 2u}}));
        EXPECT_FALSE(g.has_edge({{1u, 1u}, {2u, 2u}}));
        EXPECT_FALSE(g.has_edge({{0u, 2u}, {1u, 3u}}));
        EXPECT_FALSE(g.has_edge({{1u, 2u}, {2u, 3u}}));
    }

    TEST(GridGraphTest3, GetOutputs) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_outputs( std::pair{ 0u, 0u } )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {
                    std::pair { std::pair{0u, 0u}, std::pair{0u, 1u} },
                    std::pair { std::pair{0u, 0u}, std::pair{1u, 0u} },
                    std::pair { std::pair{0u, 0u}, std::pair{1u, 1u} }
                })
            );
        }
        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_outputs( std::pair{ 1u, 2u } )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {})
            );
        }
        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_outputs( std::pair{ 0u, 2u } )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {
                    std::pair { std::pair{0u, 2u}, std::pair{1u, 2u} }
                })
            );
        }
        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_outputs( std::pair{ 1u, 0u } )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {
                    std::pair { std::pair{1u, 0u}, std::pair{1u, 1u} }
                })
            );
        }
    }

    TEST(GridGraphTest3, GetInputs) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_inputs( std::pair{ 0u, 0u } )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {})
            );
        }
        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_inputs( std::pair{ 1u, 2u } )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {
                    std::pair { std::pair{0u, 2u}, std::pair{1u, 2u} },
                    std::pair { std::pair{1u, 1u}, std::pair{1u, 2u} },
                    std::pair { std::pair{0u, 1u}, std::pair{1u, 2u} }
                })
            );
        }
        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_inputs( std::pair{0u, 2u} )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {
                    std::pair { std::pair{0u, 1u}, std::pair{0u, 2u} }
                })
            );
        }
        {
            std::set<decltype(g)::E> actual {}; // TODO: I can't use being() and end() within set's constructor to automate this?
            for (auto _e : g.get_inputs( std::pair{1u, 0u} )) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::set<decltype(g)::E> {
                    std::pair { std::pair{0u, 0u}, std::pair{1u, 0u} }
                })
            );
        }
    }

    TEST(GridGraphTest3, GetOutputDegree) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        EXPECT_EQ(g.get_out_degree(std::pair{ 0u, 0u } ), 3);
        EXPECT_EQ(g.get_out_degree(std::pair{ 1u, 2u } ), 0);
        EXPECT_EQ(g.get_out_degree(std::pair{ 0u, 2u } ), 1);
        EXPECT_EQ(g.get_out_degree(std::pair{ 1u, 0u } ), 1);
    }

    TEST(GridGraphTest3, GetInputDegree) {
        grid_graph<std::string, std::string, unsigned int, 2u, 3u> g {};

        EXPECT_EQ(g.get_in_degree(std::pair{ 0u, 0u } ), 0);
        EXPECT_EQ(g.get_in_degree(std::pair{ 1u, 2u } ), 3);
        EXPECT_EQ(g.get_in_degree(std::pair{ 0u, 2u } ), 1);
        EXPECT_EQ(g.get_in_degree(std::pair{ 1u, 0u } ), 1);
    }
}