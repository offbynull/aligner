#include <ranges>
#include "offbynull/helpers/cartesian_product_2d_view.h"
#include "gtest/gtest.h"

namespace {
    using offbynull::helpers::cartesian_product_2d_view::cartesian_product_2d_view;

    TEST(CartesianProduct2dViewTest, ForwardRangeTest) {
        cartesian_product_2d_view<int> r{2, 2};
        static_assert(std::ranges::forward_range<decltype(r)>);
        for (const auto& [a,b] : r) {
            std::cout << a << ',' << b << ' ';
        }
        for (const auto& [a,b] : r) {
            std::cout << a << ',' << b << ' ';
        }
        std::cout << std::endl;
    }

    TEST(CartesianProduct2dViewTest, PipeTest) {
        // TODO: FIX CLASS TO SUPPORT PIPE OPERATOR
        // std::vector<std::vector<int>> vec {{1, 2}, {3, 4}, {5, 6}};
        // auto join_range {
        //     forward_range_join_view(
        //         std::move(vec)
        //     )
        //     | std::views::transform([](const auto&v) {return v*2;})
        // };
        // static_assert(std::ranges::forward_range<decltype(join_range)>);
        // for (int n : join_range) {
        //     std::cout << n << ' ';
        // }
        // for (int n : join_range) {
        //     std::cout << n << ' ';
        // }
    }
}