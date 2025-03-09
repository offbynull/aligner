#include "offbynull/aligner/scorers/pam_scorer.h"
#include "offbynull/utils.h"
#include "gtest/gtest.h"
#include <tuple>

namespace {
    using offbynull::aligner::scorers::pam_scorer::distance;
    using offbynull::aligner::scorers::pam_scorer::pam_scorer;
    using offbynull::utils::is_debug_mode;

    TEST(OASPamScorerTest, SanityTest30) {
        pam_scorer<is_debug_mode(), distance::_30, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(6, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-6, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(10, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-6, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }

    TEST(OASPamScorerTest, SanityTest70) {
        pam_scorer<is_debug_mode(), distance::_70, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(5, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-4, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(9, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-4, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }

    TEST(OASPamScorerTest, SanityTest250) {
        pam_scorer<is_debug_mode(), distance::_250, int> scorer {};
        char a_ { 'A' };
        char c_ { 'C' };
        EXPECT_EQ(2, (scorer(std::tuple<> {}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-2, (scorer(std::tuple<> {}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(12, (scorer(std::tuple<> {}, { { c_ } }, { { c_ } })));
        EXPECT_EQ(-2, (scorer(std::tuple<> {}, { { c_ } }, { { a_ } })));
    }
}