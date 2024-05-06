#include "global_aligner.h"
#include "gtest/gtest.h"
#include <iostream>
#include <format>

namespace {
    TEST(GlobalAlignerTest, TestGlobalAligner) {
        global_aligner<char> aligner {};
        const auto &[weight, alignment] {
            aligner.align(
                {'h', 'e', 'l', 'l', 'o'},
                {'m', 'e', 'l', 'l', 'o', 'w'},
                [](const std::optional<char>& v, const std::optional<char>& w) {
                    if (v == std::nullopt) {
                        return 0.0;
                    } else if (w == std::nullopt) {
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
            align1 += ch.value_or('-');
        }
        std::string align2 {};
        for (const auto& col : alignment) {
            const auto& ch = std::get<1>(col);
            align2 += ch.value_or('-');
        }
        EXPECT_EQ(align1, "hello-");
        EXPECT_EQ(align2, "mellow");
        std::cout << align1 << std::endl;
        std::cout << align2 << std::endl;
    }
}