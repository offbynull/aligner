#include "offbynull/aligner/graph/graph.h"
#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/\
diagonal_slice_slot_container_triplet.h"
#include "offbynull/aligner/scorers/simple_scorer.h"
#include "offbynull/utils.h"
#include "gtest/gtest.h"
#include <cstddef>
#include <stdfloat>
#include <string>
#include <cstddef>

namespace {
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker
        ::diagonal_slice_slot_container_triplet::diagonal_slice_slot_container_triplet;
    using offbynull::aligner::graphs::pairwise_global_alignment_graph::pairwise_global_alignment_graph;
    using offbynull::aligner::scorers::simple_scorer::simple_scorer;
    using offbynull::utils::is_debug_mode;

    TEST(OABMDiagonalSliceSlotContainerTripletTest, AccessSlots) {
        auto substitution_scorer { simple_scorer<is_debug_mode(), char, char, std::float64_t>::create_substitution(1.0f64, -1.0f64) };
        auto gap_scorer { simple_scorer<is_debug_mode(), char, char, std::float64_t>::create_gap(0.0f64) };
        std::string seq1 { "xy" };
        std::string seq2 { "abcd" };
        pairwise_global_alignment_graph<
            is_debug_mode(),
            std::size_t,
            std::float64_t,
            decltype(seq1),
            decltype(seq2),
            decltype(substitution_scorer),
            decltype(gap_scorer)
        > g {
            seq1,
            seq2,
            substitution_scorer,
            gap_scorer
        };
        diagonal_slice_slot_container_triplet<is_debug_mode(), decltype(g)> container_triplet { g };
        /*
                          1      <-- testing here
                          2 2
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5
                          6 6
                          7
        */
        EXPECT_TRUE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 4zu}).has_value());
        /*
                          1
                          2 2    <-- testing here
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5
                          6 6
                          7
        */
        container_triplet.next();
        EXPECT_TRUE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 4zu}).has_value());
        /*
                          1
                          2 2
         1 2 3 4 5        3 3 3  <-- testing here
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5
                          6 6
                          7
        */
        container_triplet.next();
        EXPECT_TRUE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 4zu}).has_value());
        /*
                          1
                          2 2
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4 <-- testing here
         3 4 5 6 7        5 5 5
                          6 6
                          7
        */
        container_triplet.next();
        EXPECT_FALSE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 4zu}).has_value());
        /*
                          1
                          2 2
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5 <-- testing here
                          6 6
                          7
        */
        container_triplet.next();
        EXPECT_FALSE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 4zu}).has_value());
        /*
                          1
                          2 2
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5
                          6 6   <-- testing here
                          7
        */
        container_triplet.next();
        EXPECT_FALSE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 4zu}).has_value());
        /*
                          1
                          2 2
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5
                          6 6
                          7     <-- testing here
        */
        container_triplet.next();
        EXPECT_FALSE(container_triplet.find({0zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 2zu}).has_value());
        EXPECT_FALSE(container_triplet.find({0zu, 3zu}).has_value());
        EXPECT_TRUE(container_triplet.find({0zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 1zu}).has_value());
        EXPECT_FALSE(container_triplet.find({1zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 3zu}).has_value());
        EXPECT_TRUE(container_triplet.find({1zu, 4zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 0zu}).has_value());
        EXPECT_FALSE(container_triplet.find({2zu, 1zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 2zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 3zu}).has_value());
        EXPECT_TRUE(container_triplet.find({2zu, 4zu}).has_value());
    }
}
