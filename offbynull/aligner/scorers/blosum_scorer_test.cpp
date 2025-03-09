#include "offbynull/aligner/scorers/blosum_scorer.h"
#include "offbynull/utils.h"
#include "gtest/gtest.h"
#include <tuple>

namespace {
    using offbynull::aligner::scorers::blosum_scorer::blosum_scorer;
    using offbynull::aligner::scorers::blosum_scorer::threshold;
    using offbynull::utils::is_debug_mode;

    TEST(OASBlosumScorerTest, SanityTest45) {
        blosum_scorer<is_debug_mode(), threshold::_45, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(12, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }

    TEST(OASBlosumScorerTest, SanityTest50) {
        blosum_scorer<is_debug_mode(), threshold::_50, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(13, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }

    TEST(OASBlosumScorerTest, SanityTest62) {
        blosum_scorer<is_debug_mode(), threshold::_62, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(4, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(0, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(9, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(0, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }

    TEST(OASBlosumScorerTest, SanityTest80) {
        blosum_scorer<is_debug_mode(), threshold::_80, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(9, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }

    TEST(OASBlosumScorerTest, SanityTest90) {
        blosum_scorer<is_debug_mode(), threshold::_90, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(9, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-1, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }
}