#ifndef OFFBYNULL_HELPERS_FORWARD_RANGE_JOIN_VIEW_H
#define OFFBYNULL_HELPERS_FORWARD_RANGE_JOIN_VIEW_H

#include <ranges>
#include <offbynull/utils.h>

namespace offbynull::helpers::forward_range_join_view {
    template<std::ranges::range R>
    class iterator;

    template<std::ranges::range R>
    class sentinel {
    public:
        sentinel(const sentinel<R> &src) = default;

        sentinel(sentinel<R> &&src) = default;

        sentinel(R& range)
        : it_{range.end()} {}

        sentinel() {}

        sentinel<R>& operator=(const sentinel<R>& rhs) = default;

        // need to provide sentinel == iterator separately
        bool operator==(const auto& other) const {
            return it_ == other.it_;
        }

    private:
        decltype(std::declval<R>().end()) it_;
        friend class iterator<R>;
    };

    template<std::ranges::range R>
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename decltype(std::declval<R>().begin())::value_type;
        using difference_type = typename decltype(std::declval<R>().begin())::difference_type;

        iterator(const iterator<R> &src) = default;

        iterator(iterator<R> &&src) = default;

        iterator(R& range)
        : it_{range.begin()} {}

        iterator() {}

        iterator<R>& operator=(const iterator<R>& rhs) = default;

        value_type operator*() const {
            return *it_;
        }

        iterator<R>& operator++() {
            ++it_;
            return *this;
        }

        iterator<R> operator++(int) {
            iterator<R> tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const auto& other) const {
            return it_ == other.it_;
        }

        bool operator!=(const auto& other) const {
            return !(*this == other);
        }

    private:
        decltype(std::declval<R>().begin()) it_;
        friend class sentinel<R>;
    };

    // Why is this class neccessary? Imagine you have some range that returns ranges (e.g. a range that returns
    // std::views::iota) and you want to use std::views::join to flatten that range. If the top-level range is returning
    // those inner ranges as a ...
    //
    //  * reference, std::views::join will end up being a std::forward_range.
    //  * object, std::views::join will end up being a std::input_range.
    //
    // The reason for this seems to be unclear. The way I understand it, std::join::views will not store copies of inner
    // ranges but will store the references to ranges? That is, if it's an object (not a reference), that object is
    // thrown away once its been fully walked over.
    template<std::ranges::range R>
    class forward_range_join_view : public std::ranges::view_interface<forward_range_join_view<R>>{
    private:
        R r;
        decltype(std::views::join(r)) r_joined;

    public:
        explicit forward_range_join_view(R&& range)
        : r{std::forward<R>(range)}
        , r_joined{std::views::join(r)} {}

        forward_range_join_view(const forward_range_join_view<R> &src) = default;
        forward_range_join_view(forward_range_join_view<R> &&src) = default;
        forward_range_join_view<R>& operator=(const forward_range_join_view<R>& other) = default;
        forward_range_join_view<R>& operator=(forward_range_join_view<R>&& other) = default;

        std::forward_iterator auto begin() {
            // INVALIDATES ANY OTHER ACTIVE ITERATOR
            r_joined = std::views::join(r);
            return iterator{r_joined};
        }

        std::sentinel_for<decltype(iterator{r_joined})> auto end() {
            return sentinel{r_joined};
        }
    };

    // Proper placement of the deduction guide
    template <std::ranges::range R>
    forward_range_join_view(R&&) -> forward_range_join_view<R>;

    struct forward_range_join_adaptor {
        template<std::ranges::range R>
        constexpr auto operator() (R&& r) const {
            return forward_range_join_view<R>(std::forward<R>(r));
        }
    };

    template<std::ranges::range R, typename Adaptor>
    auto operator|(const forward_range_join_view<R>& r, Adaptor adaptor) {
        return adaptor(std::views::all(r));
    }

    template<std::ranges::range R, typename Adaptor>
    auto operator|(forward_range_join_view<R>&& r, Adaptor adaptor) {
        return adaptor(std::views::all(std::move(r)));  // doing std::views::all(std::move(r)) causes compile error -- all() doesn't accept rvalue refs?
    }
}

#endif //OFFBYNULL_HELPERS_FORWARD_RANGE_JOIN_VIEW_H