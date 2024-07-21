#include "offbynull/aligner/scorers/blosum62_scorer.h"
#include "gtest/gtest.h"
#include <format>
#include <stdfloat>

namespace {
    using offbynull::aligner::scorers::blosum62_scorer::blosum62_scorer;

    TEST(Blosum62ScorerTest, SanityTest) {
        blosum62_scorer<int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(4, (scorer(std::tuple<>{}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(0, (scorer(std::tuple<>{}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(-4, (scorer(std::tuple<>{}, { { a_ } }, { std::nullopt })));
        EXPECT_EQ(9, (scorer(std::tuple<>{}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(0, (scorer(std::tuple<>{}, { { c_ } }, { { a_ } })));
        EXPECT_EQ(-4, (scorer(std::tuple<>{}, { { c_ } }, { std::nullopt })));;
        EXPECT_EQ(-4, (scorer(std::tuple<>{}, { std::nullopt }, { { a_ } })));
        EXPECT_EQ(-4, (scorer(std::tuple<>{}, { std::nullopt }, { { c_ } })));
        EXPECT_EQ(1, (scorer(std::tuple<>{}, { std::nullopt }, { std::nullopt })));
    }
}