#include "offbynull/aligner/scorers/pam250_scorer.h"
#include "gtest/gtest.h"
#include <format>
#include <stdfloat>

namespace {
    using offbynull::aligner::scorers::pam250_scorer::pam250_scorer;

    TEST(Pam250ScorerTest, SanityTest) {
        pam250_scorer<int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(2, (scorer(std::tuple<>{}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-2, (scorer(std::tuple<>{}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(-8, (scorer(std::tuple<>{}, { { a_ } }, { std::nullopt })));
        EXPECT_EQ(12, (scorer(std::tuple<>{}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-2, (scorer(std::tuple<>{}, { { c_ } }, { { a_ } })));
        EXPECT_EQ(-8, (scorer(std::tuple<>{}, { { c_ } }, { std::nullopt })));;
        EXPECT_EQ(-8, (scorer(std::tuple<>{}, { std::nullopt }, { { a_ } })));
        EXPECT_EQ(-8, (scorer(std::tuple<>{}, { std::nullopt }, { { c_ } })));
        EXPECT_EQ(1, (scorer(std::tuple<>{}, { std::nullopt }, { std::nullopt })));
    }
}