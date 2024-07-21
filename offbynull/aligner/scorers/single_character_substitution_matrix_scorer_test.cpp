#include "offbynull/aligner/scorers/single_character_substitution_matrix_scorer.h"
#include "gtest/gtest.h"
#include <format>
#include <stdfloat>

namespace {
    using offbynull::aligner::scorers::single_character_substitution_matrix_scorer::single_character_substitution_matrix_scorer;

    TEST(SingleCharacterSubstitutionMatrixScorerTest, SanityTest) {
        single_character_substitution_matrix_scorer<int, 5zu> scorer {
            R"(
    A   T   C   G   *
A   6  -7  -4  -3  -6
T  -7   8  -6 -10  -8
C  -4  -6   8   2 -11
G  -3 -10   2   8 -14
*  -6  -8 -11 -14  10
            )"
        };
        char a_ { 'A' };
        char c_ { 'C' };
        char t_ { 'T' };
        char g_ { 'G' };
        EXPECT_EQ(6, (scorer(std::tuple<>{}, { { a_ } }, { { a_ } })));
        EXPECT_EQ(-4, (scorer(std::tuple<>{}, { { a_ } }, { { c_ } })));
        EXPECT_EQ(-14, (scorer(std::tuple<>{}, { { g_ } }, { std::nullopt })));
        EXPECT_EQ(-8, (scorer(std::tuple<>{}, { std::nullopt }, { { t_ } })));
        EXPECT_EQ(10, (scorer(std::tuple<>{}, { std::nullopt }, { std::nullopt })));
    }
}