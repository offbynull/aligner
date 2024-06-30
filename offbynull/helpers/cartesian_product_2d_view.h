#ifndef OFFBYNULL_HELPERS_CARTESIAN_PRODUCT_2D_VIEW_H
#define OFFBYNULL_HELPERS_CARTESIAN_PRODUCT_2D_VIEW_H

#include <ranges>
#include <vector>

namespace offbynull::helpers::cartesian_product_2d_view {
    class sentinel {
    public:
        // need to provide sentinel == iterator separately
        bool operator==(const auto& i) const {
            return i == *this;
        }
    };

    template<std::integral T>
    class iterator {
        T dim1;
        T dim1_cnt;
        T dim2;
        T dim2_cnt;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<T, T>;
        using pointer = value_type*;
        using reference = value_type;
        using iterator_category = std::forward_iterator_tag;

        iterator()
        : dim1{}
        , dim1_cnt{}
        , dim2{}
        , dim2_cnt{} {}

        iterator(T dim1_, const T dim1_cnt_, T dim2_, const T dim2_cnt_)
        : dim1(dim1_)
        , dim1_cnt(dim1_cnt_)
        , dim2(dim2_)
        , dim2_cnt(dim2_cnt_) {}

        reference operator*() const {
            return std::pair<T, T> {dim1, dim2};
        }

        iterator& operator++() {
            dim1++;
            if (dim1 == dim1_cnt) {
                dim1 = static_cast<T>(0u);
                dim2++;
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            // std::cout << this->dim1 << 'x' << this->dim2 << " vs "
            //     << other.dim1 << 'x' << other.dim2
            //     << std::endl;
            return dim1 == other.dim1
                && dim1_cnt == other.dim1_cnt
                && dim2 == other.dim2
                && dim2_cnt == other.dim2_cnt;
        }

        bool operator==(const sentinel&) const {
            return dim1 == 0u
                && dim2 == dim2_cnt;
        }
    };

    // Use this as opposed to std::views::cartesian_product(std::views::iota(...), std::views::iota(...)). See
    // https://www.reddit.com/r/cpp_questions/comments/1d2qecv/use_of_views_results_in_4x_the_number_of_assembly/.
    template<std::integral T>
    class cartesian_product_2d_view : public std::ranges::view_interface<cartesian_product_2d_view<T>> {
        T dim1_cnt;
        T dim2_cnt;

        public:
        cartesian_product_2d_view(T dim1_cnt_, T dim2_cnt_)
            : dim1_cnt(dim1_cnt_)
            , dim2_cnt(dim2_cnt_) {}

        cartesian_product_2d_view(cartesian_product_2d_view<T>& other) = default;
        cartesian_product_2d_view(cartesian_product_2d_view<T>&& other) = default;
        cartesian_product_2d_view<T>& operator=(const cartesian_product_2d_view<T>& other) = default;
        cartesian_product_2d_view<T>& operator=(cartesian_product_2d_view<T>&& other) = default;

        auto begin() noexcept {
            return iterator(
                static_cast<T>(0u),
                dim1_cnt,
                static_cast<T>(0u),
                dim2_cnt
            );
        }

        auto end() const noexcept {
            return sentinel{};
        }
    };

    // Proper placement of the deduction guide
    template <std::integral T>
    cartesian_product_2d_view(T&&, T&&) -> cartesian_product_2d_view<T>;

    struct cartesian_product_2d_adaptor {
        template<std::integral T>
        constexpr auto operator() (T dim1_cnt, T dim2_cnt) const {
            return cartesian_product_2d_view<T>(dim1_cnt, dim2_cnt);
        }
    };

    template<std::integral T, typename Adaptor>
    auto operator|(const cartesian_product_2d_view<T>& r, Adaptor adaptor) {
        return adaptor(std::views::all(r));
    }

    template<std::integral T, typename Adaptor>
    auto operator|(cartesian_product_2d_view<T>&& r, Adaptor adaptor) {
        return adaptor(std::views::all(std::move(r)));  // doing std::views::all(std::move(r)) causes compile error -- all() doesn't accept rvalue refs?
    }
}

#endif //OFFBYNULL_HELPERS_CARTESIAN_PRODUCT_2D_VIEW_H