#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <ranges>
#include <iterator>
#include <utility>

// Use this view because std::views::concat isn't available in C++23 and the ranges-v3 lib doesn't play nice with ranges in the standard library.
// Note that this view OWNS the ranges you pass into it.
template<typename R1, typename R2>
class concat_view : public std::ranges::view_interface<concat_view<R1, R2>> {
    R1 first_range;
    R2 second_range;

private:
    class sentinel {};

public:
    template<typename T1, typename T2>
    concat_view(T1&& first, T2&& second)
        : first_range(std::forward<T1>(first)), second_range(std::forward<T2>(second)) {}


    template<typename It1, typename S1, typename It2, typename S2>
    class iterator {
        It1 it1;
        S1 end1;
        It2 it2;
        S2 end2;
        bool isFirst;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::common_type_t<decltype(*std::declval<It1>()), decltype(*std::declval<It2>())>;
        using pointer = value_type*;
        using reference = value_type;
        using iterator_category = std::input_iterator_tag;

        iterator(It1 first, S1 last1, It2 second, S2 last2, bool firstRange)
            : it1(first), end1(last1), it2(second), end2(last2), isFirst(firstRange) {}

        reference operator*() {
            if (isFirst) {
                if (it1 != end1) {
                    return *it1;
                } else {
                    isFirst = false;
                    return *it2;
                }
            } else {
                return *it2;
            }
        }

        iterator& operator++() {
            if (isFirst) {
                if (++it1 == end1) {
                    isFirst = false;
                }
            } else {
                ++it2;
            }
            return *this;
        }

        bool operator==(const iterator& other) const {
            return isFirst == other.isFirst && (isFirst ? it1 == other.it1 : it2 == other.it2);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        bool operator==(const concat_view<R1, R2>::sentinel&) const {
            return !isFirst && it2 == end2;
        }

        bool operator!=(const concat_view<R1, R2>::sentinel& s) const {
            return !(*this == s);
        }
    };

    auto begin() {
        return iterator(std::ranges::begin(first_range), std::ranges::end(first_range),
                        std::ranges::begin(second_range), std::ranges::end(second_range), true);
    }

    sentinel end() {
        return {};
    }
};

// Template argument deduction guides
template<typename T1, typename T2>
concat_view(T1&&, T2&&) -> concat_view<std::decay_t<T1>, std::decay_t<T2>>;

// Helper to detect if type T has a nested iterator type
template <typename, typename = std::void_t<>>
struct has_iterator : std::false_type {};

template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {};

template<typename T>
struct TypeDisplayer;  // Intentionally undefined

#endif //UTILS_H
