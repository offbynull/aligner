#include <ranges>
#include <vector>
#include <iostream>
#include "offbynull/helpers/variant_bidirectional_view.h"
#include "offbynull/utils.h"
#include "gtest/gtest.h"

namespace {
    using offbynull::helpers::variant_bidirectional_view::variant_bidirectional_view;
    using offbynull::utils::is_debug_mode;

    TEST(OHVariantBidirectionalViewTest, SanityTest) {
        using R1 = decltype(std::vector<int> {});
        using R2 = decltype(std::views::iota(0 ,0));
        {
            variant_bidirectional_view<R1, R2> r_ { std::views::iota(1, 10) };
            EXPECT_NE(r_.begin(), r_.end());
            for (const auto& n : r_) {
                std::cout << n << ' ';
            }
        }
        {
            variant_bidirectional_view<R1, R2> r_ {
                std::vector<int> { -1, -2, -3 }
            };
            EXPECT_NE(r_.begin(), r_.end());
            for (const auto& n : r_) {
                std::cout << n << ' ';
            }
        }
    }
}