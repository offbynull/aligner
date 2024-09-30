#ifndef OFFBYNULL_HELPERS_JOIN_BIDIRECTIONAL_VIEW_H
#define OFFBYNULL_HELPERS_JOIN_BIDIRECTIONAL_VIEW_H

#include <ranges>
#include <iterator>
#include <cstdint>
#include <utility>
#include <optional>
#include <type_traits>
#include "offbynull/utils.h"
#include "offbynull/concepts.h"

namespace offbynull::helpers::join_bidirectional_view {
    using offbynull::concepts::bidirectional_range_of_bidirectional_range;

    struct begin_marker {};
    struct end_marker {};

    template <
        std::bidirectional_iterator OUTER_IT
    >
    struct outer_pack {
        OUTER_IT it_begin;
        OUTER_IT it_end;
        OUTER_IT it;

        outer_pack(OUTER_IT& it_begin_, OUTER_IT& it_end_, OUTER_IT& it_)
        : it_begin { it_begin_ }
        , it_end { it_end_ }
        , it { it_ } {}

        static std::optional<outer_pack> from_begin(OUTER_IT outer_it_begin, OUTER_IT outer_it_end) {
            return { { outer_it_begin, outer_it_end, outer_it_begin } };
        }

        static std::optional<outer_pack> from_end(OUTER_IT outer_it_begin, OUTER_IT outer_it_end) {
            return { { outer_it_begin, outer_it_end, outer_it_end } };
        }
    };

    template <
        std::bidirectional_iterator OUTER_IT
    >
    struct inner_pack {
        using INNER_R = std::remove_cvref_t<decltype(*std::declval<OUTER_IT>())>;
        using INNER_IT = std::remove_cvref_t<decltype(std::declval<INNER_R>().begin())>;

        inner_pack(OUTER_IT& outer_it, begin_marker /*unused*/)
        : range { *outer_it }
        , it_begin { range.begin() }
        , it_end { range.end() }
        , it { it_begin } {}

        inner_pack(OUTER_IT& outer_it, end_marker /*unused*/)
        : range { *outer_it }
        , it_begin { range.begin() }
        , it_end { range.end() }
        , it { it_end } {}

        // Can't have default implementation of copy consturctor/assignment because, even though range is copied and iterators are copied,
        // the iterator copies will point the original range as opposed to the new range.
        inner_pack(const inner_pack& src)
        : range { src.range }
        , it_begin { range.begin() }
        , it_end { range.end() }
        , it { it_begin } {
            std::advance(it, std::ranges::distance(src.it_begin, src.it));  // Move it by appropriate amount
        }

        inner_pack& operator=(const inner_pack &rhs) noexcept {
            range = rhs.range;
            it_begin = range.begin();
            it_end = range.end();
            it = it_begin;
            std::advance(it, std::ranges::distance(rhs.it_begin, rhs.it));  // Move it by appropriate amount
            return *this;
        }

        // Can't have default implementation of move consturctor/assignment because, even though range is moved and iterators are moved,
        // the moved iterator will probably still point to the original range in some way as opposed to the new range? Using default
        // implementation is causing use-after-destroyed in some places so custom implementation being used here.
        inner_pack(inner_pack&& src) noexcept
        : range { std::move(src.range) }
        , it_begin { range.begin() }
        , it_end { range.end() }
        , it { it_begin } {
            std::advance(it, std::ranges::distance(src.it_begin, src.it));  // Move it by appropriate amount
        }

        inner_pack& operator=(inner_pack &&rhs) noexcept {
            range = std::move(rhs.range);
            it_begin = range.begin();
            it_end = range.end();
            it = it_begin;
            std::advance(it, std::ranges::distance(rhs.it_begin, rhs.it));  // Move it by appropriate amount
            return *this;
        }

        INNER_R range;
        INNER_IT it_begin;
        INNER_IT it_end;
        INNER_IT it;

        static std::optional<inner_pack> from_begin(OUTER_IT outer_it_begin, OUTER_IT outer_it_end) {
            if (outer_it_begin == outer_it_end) {
                return { std::nullopt };
            }
            return { { outer_it_begin, begin_marker {} } };
        }

        static std::optional<inner_pack> from_end(OUTER_IT outer_it_begin, OUTER_IT outer_it_end) {
            if (outer_it_begin == outer_it_end) {
                return { std::nullopt };
            }
            OUTER_IT outer_it { outer_it_end };
            --outer_it;
            return { { outer_it, end_marker {} } };
        }
    };

    template <
        std::bidirectional_iterator OUTER_IT
    >
    class iterator {
    private:
        using INNER_R = std::remove_cvref_t<decltype(*std::declval<OUTER_IT>())>;
        using INNER_IT = std::remove_cvref_t<decltype(std::declval<INNER_R>().begin())>;

        std::optional<outer_pack<OUTER_IT>> outer;
        std::optional<inner_pack<OUTER_IT>> inner;  // Set to nullopt if !outer.has_value() || outer->it == outer->it_end

        void skip_empty_ranges_forward() {
            if (!outer.has_value()) {
                return;  // No outer range
            }
            while (outer->it != outer->it_end) {
                if (inner->it_begin == inner->it_end) {
                    ++outer->it;
                    if (outer->it != outer->it_end) {
                        inner = { inner_pack(outer->it, begin_marker {}) };
                    } else {
                        inner = { std::nullopt };
                    }
                } else {
                    break;
                }
            }
        }

        void skip_empty_ranges_backward() {
            if (!outer.has_value()) {
                return;
            }
            OUTER_IT outer_it { outer->it };
            std::optional<inner_pack<OUTER_IT>> inner_ = inner;
            do {
                if (!inner_.has_value() || inner_->it_begin == inner_->it_end) {
                    --outer_it;
                    inner_ = { inner_pack(outer_it, end_marker {}) };
                } else {
                    break;
                }
            } while (outer_it != outer->it_begin);

            if (outer_it != outer->it) {
                outer->it = outer_it;
                inner = inner_;
            }
        }

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::remove_cvref_t<typename INNER_IT::value_type>;
        // using reference = typename INNER_IT::reference;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator()
        : outer { std::nullopt }
        , inner { std::nullopt } {}

        iterator(OUTER_IT outer_it_begin_, OUTER_IT outer_it_end_, begin_marker /*unused*/)
        : outer { outer_pack<OUTER_IT>::from_begin(outer_it_begin_, outer_it_end_) }
        , inner { inner_pack<OUTER_IT>::from_begin(outer->it_begin, outer->it_end) } {
            skip_empty_ranges_forward();
        }

        iterator(OUTER_IT outer_it_begin_, OUTER_IT outer_it_end_, end_marker /*unused*/)
        : outer { outer_pack<OUTER_IT>::from_end(outer_it_begin_, outer_it_end_) }
        , inner { std::nullopt } {}

        iterator(const iterator<OUTER_IT> &src) = default;
        iterator(iterator<OUTER_IT> &&src) noexcept = default;
        iterator<OUTER_IT>& operator=(const iterator<OUTER_IT>& rhs) = default;

        value_type operator*() const {
            return *inner->it;
        }

        iterator& operator++() {
            ++inner->it;
            if (inner->it == inner->it_end) {
                ++outer->it;
                if (outer->it != outer->it_end) {
                    inner = { outer->it, begin_marker {} };
                    skip_empty_ranges_forward();
                } else {
                    inner = { std::nullopt };
                }
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator& operator--() {
            if (!inner.has_value() || inner->it == inner->it_begin) {
                --outer->it;
                inner = { outer->it, end_marker {} };
                skip_empty_ranges_backward();
            }
            --inner->it;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            // If no outer, always return true.
            if (!outer.has_value()) {
                return true;
            }
            // Because inner is always going to be a copy, directly returning ...
            //
            //   outer_it == other.outer_it && inner.it == other.inner.it
            //
            // ... won't work. Since this's inner and other's inner are copie sof each other, their iterator equality will always be false.
            // To work around this, first check if both this and other have the same outer between them (if yes, their outer_its will be
            // equal because both outer_its will be pointing to the same collection).
            if (outer->it_begin != other.outer->it_begin) {
                return false;  // Different outer objects detected
            }
            // Then, check if both are ended. This is the most important case to handle, so it should be performant.
            bool this_ended { outer->it == outer->it_end };
            bool other_ended { other.outer->it == other.outer->it_end };
            if (other_ended) {
                return this_ended;
            } else if (this_ended) {
                return other_ended;
            }
            // If not ended, get the distance between the inner iterators and test that instead. This will always work, but depending on the
            // type of inner.range / inner.it, it may not be performant (should be fine because this case will almost never be hit).
            if (outer->it != other.outer->it) {  // Both at same outer position?
                return false;
            }
            if (inner.has_value() != other.inner.has_value()) {  // Both have inner range?
                return false;
            }
            return std::ranges::distance(inner->it_begin, inner->it) == std::ranges::distance(other.inner->it_begin, other.inner->it);
        }
    };

    template <bidirectional_range_of_bidirectional_range R>
    class join_bidirectional_view : public std::ranges::view_interface<join_bidirectional_view<R>> {
    private:
        R range;

    public:
        join_bidirectional_view(R&& range_)
        : range(std::forward<R>(range_)) {}

        join_bidirectional_view(const join_bidirectional_view<R>& other) = default;
        join_bidirectional_view(join_bidirectional_view<R>&& other) noexcept = default;
        join_bidirectional_view<R>& operator=(const join_bidirectional_view<R>& other) = default;
        join_bidirectional_view<R>& operator=(join_bidirectional_view<R>&& other) = default;

        std::bidirectional_iterator auto begin() {
            return iterator(
                std::ranges::begin(range),
                std::ranges::end(range),
                begin_marker {}
            );
        }

        std::bidirectional_iterator auto end() {
            return iterator(
                std::ranges::begin(range),
                std::ranges::end(range),
                end_marker {}
            );
        }
    };

    // Adaptors to use in pipe operator chains. Type deduction guide is required for this?
    template <bidirectional_range_of_bidirectional_range R>
    join_bidirectional_view(R&&) -> join_bidirectional_view<R>;

    struct join_bidirectional_view_adaptor {
        template <bidirectional_range_of_bidirectional_range R>
        constexpr auto operator()(R&& r) const {
            return join_bidirectional_view<R>(std::forward<R>(r));
        }
    };

    template<bidirectional_range_of_bidirectional_range R>
    auto operator|(R&& r, join_bidirectional_view_adaptor const& adaptor) {
        return adaptor(std::forward<R>(r));
    }
}

#endif //OFFBYNULL_HELPERS_JOIN_BIDIRECTIONAL_VIEW_H