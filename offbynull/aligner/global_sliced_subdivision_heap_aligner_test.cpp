#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include "offbynull/aligner/global_sliced_subdivision_heap_aligner.h"
#include "offbynull/aligner/scorers/simple_scorer.h"
#include "offbynull/utils.h"
#include "gtest/gtest.h"

namespace {
    using offbynull::aligner::global_sliced_subdivision_heap_aligner::sliced_subdivision_heap_allocation_strategy_config;
    using offbynull::aligner::global_sliced_subdivision_heap_aligner::global_sliced_subdivision_heap_aligner;
    using offbynull::aligner::scorers::simple_scorer::simple_scorer;
    using offbynull::utils::is_debug_mode;

    TEST(OAGlobalSlicedSubdivisionHeapAlignerTest, SanityTest) {
        global_sliced_subdivision_heap_aligner<is_debug_mode()> aligner {};
        auto substitution_scorer { simple_scorer<is_debug_mode(), std::size_t, char, char, std::size_t>::create_substitution(1zu, 0zu) };
        auto gap_scorer { simple_scorer<is_debug_mode(), std::size_t, char, char, std::size_t>::create_gap(0zu) };
        std::string down { "panama" };
        std::string right { "banana" };
        const auto& [path, score] {
            aligner.align(down, right, substitution_scorer, gap_scorer, 0zu)
        };
        EXPECT_EQ(4zu, score);
        std::cout << score << std::endl;
    }
}