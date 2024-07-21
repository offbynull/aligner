#include "offbynull/aligner/scorers/blosum50_scorer.h"
#include "gtest/gtest.h"
#include <format>
#include <stdfloat>

namespace {
    using offbynull::aligner::scorers::blosum50_scorer::blosum50_scorer;

    TEST(Blosum50ScorerTest, SanityTest) {
        blosum50_scorer<int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<>{}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<>{}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { { a_ } }, { std::nullopt })));
        EXPECT_EQ(13, (scorer(std::tuple<>{}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<>{}, { { c_ } }, { { a_ } })));
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { { c_ } }, { std::nullopt })));;
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { std::nullopt }, { { a_ } })));
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { std::nullopt }, { { c_ } })));
        EXPECT_EQ(1, (scorer(std::tuple<>{}, { std::nullopt }, { std::nullopt })));
    }
}