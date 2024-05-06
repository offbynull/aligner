#include "global_aligner_2.h"
#include "gtest/gtest.h"
#include <iostream>

namespace {
    TEST(GlobalAlignerTest, TestCopyGlobalAligner) {
        static_assert (sizeof(std::reference_wrapper<char>) > sizeof(char), "Expected to be less");
        global_aligner<char> aligner {};
        const auto &[weight, alignment] {
            aligner.align(
                {'h', 'e', 'l', 'l', 'o'},
                {'m', 'e', 'l', 'l', 'o', 'w'},
                [](const auto& v, const auto& w) {
                    if (v == boost::none) {
                        return 0.0;
                    } else if (w == boost::none) {
                        return 0.0;
                    } else {
                        return *v == *w ? 1.0 : 0.0;
                    }
                }
            )
        };
        std::string align1 {};
        for (const auto& col : alignment) {
            const auto& ch = std::get<0>(col);
            align1 += ch.get_value_or('-');
        }
        std::string align2 {};
        for (const auto& col : alignment) {
            const auto& ch = std::get<1>(col);
            align2 += ch.get_value_or('-');
        }
        EXPECT_EQ(align1, "hello-");
        EXPECT_EQ(align2, "mellow");
        std::cout << align1 << std::endl;
        std::cout << align2 << std::endl;
    }

    TEST(GlobalAlignerTest, TestReferenceGlobalAligner) {
        struct custom_elem {
            char v;
            char discarded1[100] {};
            bool operator==(const custom_elem& rhs) const = default;
        };

        static_assert (sizeof(std::reference_wrapper<custom_elem>) < sizeof(custom_elem), "Expected to be more");
        global_aligner<custom_elem> aligner {};
        std::vector<custom_elem> v { {'h'}, {'e'}, {'l'}, {'l'}, {'o'} };
        std::vector<custom_elem> w { {'m'}, {'e'}, {'l'}, {'l'}, {'o'}, {'w'} };
        const auto &[weight, alignment] {
            aligner.align(
                v,
                w,
                [](const auto& v, const auto& w) {
                    if (v == boost::none) {
                        return 0.0;
                    } else if (w == boost::none) {
                        return 0.0;
                    } else {
                        return *v == *w ? 1.0 : 0.0;
                    }
                }
            )
        };
        std::string align1 {};
        for (const auto& col : alignment) {
            const auto& ch = std::get<0>(col);
            align1 += ch.get_value_or(custom_elem {'-'}).v;
        }
        std::string align2 {};
        for (const auto& col : alignment) {
            const auto& ch = std::get<1>(col);
            align2 += ch.get_value_or(custom_elem {'-'}).v;
        }
        EXPECT_EQ(align1, "hello-");
        EXPECT_EQ(align2, "mellow");
        std::cout << align1 << std::endl;
        std::cout << align2 << std::endl;
    }
}