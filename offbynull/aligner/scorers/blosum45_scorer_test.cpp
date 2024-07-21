#include "offbynull/aligner/scorers/blosum45_scorer.h"
#include "gtest/gtest.h"
#include <format>
#include <stdfloat>

namespace {
    using offbynull::aligner::scorers::blosum45_scorer::blosum45_scorer;

    TEST(Blosum45ScorerTest, SanityTest) {
        blosum45_scorer<int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<>{}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<>{}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { { a_ } }, { std::nullopt })));
        EXPECT_EQ(12, (scorer(std::tuple<>{}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<>{}, { { c_ } }, { { a_ } })));
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { { c_ } }, { std::nullopt })));;
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { std::nullopt }, { { a_ } })));
        EXPECT_EQ(-5, (scorer(std::tuple<>{}, { std::nullopt }, { { c_ } })));
        EXPECT_EQ(1, (scorer(std::tuple<>{}, { std::nullopt }, { std::nullopt })));
    }
}