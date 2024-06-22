#include <cstddef>
#include <algorithm>
#include "offbynull/aligner/graph/graph.h"
#include "offbynull/aligner/graph/pairwise_alignment_graph.h"
#include "offbynull/aligner/graph/grid_container_creators.h"
#include "offbynull/aligner/graphs/pairwise_local_alignment_graph.h"
#include "offbynull/aligner/graphs/middle_sliceable_pairwise_alignment_graph.h"
#include "gtest/gtest.h"

namespace {
    using offbynull::aligner::graphs::pairwise_local_alignment_graph::pairwise_local_alignment_graph;
    using offbynull::aligner::graphs::pairwise_local_alignment_graph::edge;
    using offbynull::aligner::graphs::pairwise_local_alignment_graph::edge_type;
    using offbynull::aligner::graphs::middle_sliceable_pairwise_alignment_graph::middle_sliceable_pairwise_alignment_graph;

    template <typename BACKING_G>
    struct created_graph {
        BACKING_G backing_g;
        middle_sliceable_pairwise_alignment_graph<BACKING_G> middle_g;

        created_graph(
            BACKING_G _backing_g,
            unsigned int down_offset,
            unsigned int right_offset,
            unsigned int down_cnt,
            unsigned int right_cnt
        )
        : backing_g {_backing_g}
        , middle_g {backing_g, down_offset, right_offset, down_cnt, right_cnt} {}
    };

    template<typename ED, typename INDEX = unsigned int, bool error_check = true>
    auto create_vector(INDEX down_cnt, INDEX right_cnt) {
        using G = pairwise_local_alignment_graph<
                std::tuple<>,
                ED,
                INDEX,
                offbynull::aligner::graph::grid_container_creators::vector_grid_container_creator<std::tuple<>, INDEX>,
                offbynull::aligner::graph::grid_container_creators::vector_grid_container_creator<ED, INDEX>,
                error_check
            >;
        return created_graph<G> {
            G {
                down_cnt,
                right_cnt
            },
            1u,
            1u,
            down_cnt - 2u,
            right_cnt - 2u
        };
    }

    TEST(MiddlePairwiseAlignmentGraphTest, ConceptCheck) {
        using G = decltype(create_vector<float>(4u, 4u).middle_g);
        static_assert(offbynull::aligner::graph::graph::readable_graph<G>);
        static_assert(offbynull::aligner::graph::pairwise_alignment_graph::readable_parwise_alignment_graph<G>);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, ListNodes) {
        auto x = [](auto&& g) {
            auto n = g.get_nodes();
            EXPECT_EQ(
                std::set(n.begin(), n.end()),
                (std::set {
                    std::pair{1u, 1u}, std::pair{1u, 2u},
                    std::pair{2u, 1u}, std::pair{2u, 2u},
                })
            );
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, ListEdges) {
        auto x = [](auto&& g) {
            using E = typename std::remove_reference_t<decltype(g)>::E;

            auto e = g.get_edges();
            std::multiset<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
            for (auto _e : e) {
                actual.insert(_e);
            }
            EXPECT_EQ(
                actual,
                (std::multiset<E> {
                    edge { edge_type::NORMAL, std::pair { std::pair{1u, 1u}, std::pair{1u, 2u} } },
                    edge { edge_type::NORMAL, std::pair { std::pair{1u, 1u}, std::pair{2u, 2u} } },
                    edge { edge_type::NORMAL, std::pair { std::pair{1u, 1u}, std::pair{2u, 1u} } },
                    edge { edge_type::NORMAL, std::pair { std::pair{1u, 2u}, std::pair{2u, 2u} } },
                    edge { edge_type::NORMAL, std::pair { std::pair{2u, 1u}, std::pair{2u, 2u} } },
                })
            );
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, NodesExist) {
        auto x = [](auto&& g) {
            EXPECT_FALSE(g.has_node({0u, 0u}));
            EXPECT_FALSE(g.has_node({0u, 1u}));
            EXPECT_FALSE(g.has_node({0u, 2u}));
            EXPECT_FALSE(g.has_node({0u, 3u}));
            EXPECT_FALSE(g.has_node({1u, 0u}));
            EXPECT_TRUE(g.has_node({1u, 1u}));
            EXPECT_TRUE(g.has_node({1u, 2u}));
            EXPECT_FALSE(g.has_node({1u, 3u}));
            EXPECT_FALSE(g.has_node({2u, 0u}));
            EXPECT_TRUE(g.has_node({2u, 1u}));
            EXPECT_TRUE(g.has_node({2u, 2u}));
            EXPECT_FALSE(g.has_node({2u, 3u}));
            EXPECT_FALSE(g.has_node({3u, 0u}));
            EXPECT_FALSE(g.has_node({3u, 1u}));
            EXPECT_FALSE(g.has_node({3u, 2u}));
            EXPECT_FALSE(g.has_node({3u, 3u}));
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, EdgesExist) {
        auto x = [](auto&& g) {
            EXPECT_FALSE(g.has_edge({edge_type::NORMAL, {{0u, 0u}, {0u, 1u}}}));
            EXPECT_FALSE(g.has_edge({edge_type::NORMAL, {{0u, 1u}, {0u, 2u}}}));
            EXPECT_TRUE(g.has_edge({ edge_type::NORMAL, { {1u, 1u}, {1u, 2u} } }));
            EXPECT_TRUE(g.has_edge({ edge_type::NORMAL, { {1u, 1u}, {2u, 2u} } }));
            EXPECT_TRUE(g.has_edge({ edge_type::NORMAL, { {1u, 1u}, {2u, 1u} } }));
            EXPECT_TRUE(g.has_edge({ edge_type::NORMAL, { {1u, 2u}, {2u, 2u} } }));
            EXPECT_TRUE(g.has_edge({ edge_type::NORMAL, { {2u, 1u}, {2u, 2u} } }));
            EXPECT_FALSE(g.has_edge({edge_type::NORMAL, {{6u, 7u}, {6u, 8u}}}));
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, GetOutputs) {
        auto x = [](auto&& g) {
            using E = typename std::remove_reference_t<decltype(g)>::E;

            {
                std::vector<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
                for (auto _e : g.get_outputs( { 1u, 1u } )) {
                    actual.push_back(_e);
                }
                std::sort(actual.begin(), actual.end());
                std::vector<E> expected {
                    { edge_type::NORMAL, {{1u, 1u}, {1u, 2u} } },
                    { edge_type::NORMAL, {{1u, 1u}, {2u, 1u} } },
                    { edge_type::NORMAL, {{1u, 1u}, {2u, 2u} } }
                };
                EXPECT_EQ(actual, expected);
            }
            {
                std::vector<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
                for (auto _e : g.get_outputs( { 2u, 2u } )) {
                    actual.push_back(_e);
                }
                std::sort(actual.begin(), actual.end());
                EXPECT_EQ(
                    actual,
                    (std::vector<E> {})
                );
            }
            {
                std::vector<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
                for (auto _e : g.get_outputs( { 1u, 2u } )) {
                    actual.push_back(_e);
                }
                std::sort(actual.begin(), actual.end());
                EXPECT_EQ(
                    actual,
                    (std::vector<E> {
                        { edge_type::NORMAL, { {1u, 2u}, {2u, 2u} } }
                    })
                );
            }
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, GetInputs) {
        auto x = [](auto&& g) {
            using E = typename std::remove_reference_t<decltype(g)>::E;

            {
                std::vector<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
                for (auto _e : g.get_inputs( std::pair{ 1u, 1u } )) {
                    actual.push_back(_e);
                }
                EXPECT_EQ(
                    actual,
                    (std::vector<E> {})
                );
            }
            {
                std::vector<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
                for (auto _e : g.get_inputs( { 2u, 2u } )) {
                    actual.push_back(_e);
                }
                std::sort(actual.begin(), actual.end());
                EXPECT_EQ(
                    actual,
                    (std::vector<E> {
                        { edge_type::NORMAL, {{1u, 1u}, {2u, 2u} } },
                        { edge_type::NORMAL, {{1u, 2u}, {2u, 2u} } },
                        { edge_type::NORMAL, {{2u, 1u}, {2u, 2u} } }
                    })
                );
            }
            {
                std::vector<E> actual {}; // TODO: Can't pass being() and end() to constructor to automate this? Doesn't like end() with sentinel type
                for (auto _e : g.get_inputs( std::pair{1u, 2u} )) {
                    actual.push_back(_e);
                }
                std::sort(actual.begin(), actual.end());
                EXPECT_EQ(
                    actual,
                    (std::vector<E> {
                        { edge_type::NORMAL, { {1u, 1u}, {1u, 2u} } }
                    })
                );
            }
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, GetOutputDegree) {
        auto x = [](auto&& g) {
            EXPECT_EQ(g.get_out_degree(std::pair{ 1u, 1u } ), 3);
            EXPECT_EQ(g.get_out_degree(std::pair{ 1u, 2u } ), 1);
            EXPECT_EQ(g.get_out_degree(std::pair{ 2u, 1u } ), 1);
            EXPECT_EQ(g.get_out_degree(std::pair{ 2u, 2u } ), 0);
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, GetInputDegree) {
        auto x = [](auto&& g) {
            EXPECT_EQ(g.get_in_degree(std::pair{ 1u, 1u } ), 0);
            EXPECT_EQ(g.get_in_degree(std::pair{ 1u, 2u } ), 1);
            EXPECT_EQ(g.get_in_degree(std::pair{ 2u, 1u } ), 1);
            EXPECT_EQ(g.get_in_degree(std::pair{ 2u, 2u } ), 3);
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }

    TEST(MiddlePairwiseAlignmentGraphTest, SlicedWalk) {
        auto to_vector {
            [](auto &&r) {
                auto it { r.begin() };
                std::vector<std::decay_t<decltype(*it)>> ret {};
                while (it != r.end()) {
                    ret.push_back(*it);
                    ++it;
                }
                return ret;
            }
        };

        auto x = [&](auto&& g) {
            using G = std::decay_t<decltype(g)>;
            using N = typename G::N;
            using E = typename G::E;

            EXPECT_EQ(G::slice_nodes_capacity(g.grid_down_cnt, g.grid_right_cnt), 2zu);
            EXPECT_EQ(g.slice_first_node(0u), (N { 1u, 1u }));
            EXPECT_EQ(g.slice_last_node(0u), (N { 1u, 2u }));
            EXPECT_EQ(g.slice_first_node(1u), (N { 2u, 1u }));
            EXPECT_EQ(g.slice_last_node(1u), (N { 2u, 2u }));

            EXPECT_EQ(G::resident_nodes_capacity(g.grid_down_cnt, g.grid_right_cnt), 2zu); // directly proxied from backing graph
            auto resident_nodes { g.resident_nodes() };
            EXPECT_EQ(std::distance(resident_nodes.begin(), resident_nodes.end()), 0zu);
            auto resident_nodes_it { resident_nodes.begin() };
            EXPECT_EQ(resident_nodes_it, resident_nodes.end());

            EXPECT_EQ(g.slice_next_node(N { 1u, 1u }), (N { 1u, 2u }));
            EXPECT_EQ(g.slice_next_node(N { 2u, 1u }), (N { 2u, 2u }));

            EXPECT_EQ(g.slice_prev_node(N { 2u, 2u }), (N { 2u, 1u }));
            EXPECT_EQ(g.slice_prev_node(N { 1u, 2u }), (N { 1u, 1u }));

            EXPECT_EQ(
                to_vector(g.outputs_to_residents(N { 1u, 1u })),
                (std::vector<E> {})
            );
            EXPECT_EQ(
                to_vector(g.outputs_to_residents(N { 1u, 2u })),
                (std::vector<E> {})
            );
            EXPECT_EQ(
                to_vector(g.outputs_to_residents(N { 2u, 1u })),
                (std::vector<E> {})
            );
            EXPECT_EQ(
                to_vector(g.outputs_to_residents(N { 2u, 2u })),
                (std::vector<E> {})
            );

            EXPECT_EQ(
                to_vector(g.inputs_from_residents(N { 2u, 2u })),
                (std::vector<E> {})
            );
            EXPECT_EQ(
                to_vector(g.inputs_from_residents(N { 2u, 1u })),
                (std::vector<E> {})
            );
            EXPECT_EQ(
                to_vector(g.inputs_from_residents(N { 1u, 2u })),
                (std::vector<E> {})
            );
            EXPECT_EQ(
                to_vector(g.inputs_from_residents(N { 1u, 1u })),
                (std::vector<E> {})
            );
        };
        x(create_vector<float>(4u, 4u).middle_g);
    }
}