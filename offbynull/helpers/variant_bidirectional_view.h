#ifndef OFFBYNULL_HELPERS_VARIANT_BIDIRECTIONAL_VIEW_H
#define OFFBYNULL_HELPERS_VARIANT_BIDIRECTIONAL_VIEW_H

#include <ranges>
#include <utility>
#include <type_traits>
#include <iterator>
#include <variant>
#include <tuple>
#include <concepts>
#include "offbynull/utils.h"

namespace offbynull::helpers::variant_bidirectional_view {
    using offbynull::utils::wider_numeric;

    // forward decl
    template<std::ranges::bidirectional_range R1, std::ranges::bidirectional_range R2, bool const_>
    class variant_bidirectional_iterator;

    template<std::ranges::bidirectional_range R1, std::ranges::bidirectional_range R2, bool const_>
    class variant_bidirectional_sentinel {
    private:
        using R1_ = std::conditional_t<const_, const R1, R1>;
        using R2_ = std::conditional_t<const_, const R2, R2>;

        using S1 = std::ranges::sentinel_t<R1_>;
        using S2 = std::ranges::sentinel_t<R2_>;

        std::variant<S1, S2> it_;

    public:
        variant_bidirectional_sentinel() = default;
        variant_bidirectional_sentinel(const variant_bidirectional_sentinel<R1, R2, const_> &src) = default;
        variant_bidirectional_sentinel(variant_bidirectional_sentinel<R1, R2, const_> &&src) noexcept = default;
        // in_place_index used to disambiguate when S1 and S2 are same type.
        variant_bidirectional_sentinel(std::in_place_index_t<0>, S1 s) : it_(std::in_place_index<0>, s) {}
        variant_bidirectional_sentinel(std::in_place_index_t<1>, S2 s) : it_(std::in_place_index<1>, s) {}

        friend class variant_bidirectional_iterator<R1, R2, const_>;
    };
    
    template<std::ranges::bidirectional_range R1, std::ranges::bidirectional_range R2, bool const_>
    class variant_bidirectional_iterator {
    private:
        using R1_ = std::conditional_t<const_, const R1, R1>;
        using R2_ = std::conditional_t<const_, const R2, R2>;

        using I1 = std::ranges::iterator_t<R1_>;
        using I2 = std::ranges::iterator_t<R2_>;

        std::variant<I1, I2> it_;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::common_type_t<
            std::iter_value_t<I1>,
            std::iter_value_t<I2>
        >;
        using difference_type = wider_numeric<
            std::iter_difference_t<I1>,
            std::iter_difference_t<I2>
        >::type;
        using reference = std::common_reference_t<
            std::iter_reference_t<I1>,
            std::iter_reference_t<I2>
        >; // If either iterator does not return a reference, reference should be a value type due to how iter_reference_t works?

        variant_bidirectional_iterator() = default;
        variant_bidirectional_iterator(const variant_bidirectional_iterator<R1, R2, const_> &src) = default;
        variant_bidirectional_iterator(variant_bidirectional_iterator<R1, R2, const_> &&src) noexcept = default;
        // in_place_index used to disambiguate when I1 and I2 are same type.
        variant_bidirectional_iterator(std::in_place_index_t<0>, I1 s) : it_(std::in_place_index<0>, s) {}
        variant_bidirectional_iterator(std::in_place_index_t<1>, I2 s) : it_(std::in_place_index<1>, s) {}

        // If either iterator does not return a reference, reference should be a value type due to how iter_reference_t works?
        reference operator*() const {
            if (std::holds_alternative<I1>(it_)) {
                return *std::get<I1>(it_);
            }
            return *std::get<I2>(it_);
        }

        variant_bidirectional_iterator<R1, R2, const_>& operator++() {
            std::visit([](auto& i) { ++i; }, it_);
            return *this;
        }
        variant_bidirectional_iterator<R1, R2, const_> operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        variant_bidirectional_iterator<R1, R2, const_>& operator--() {
            std::visit([](auto& i) { --i; }, it_);
            return *this;
        }
        variant_bidirectional_iterator<R1, R2, const_> operator--(int) {
            auto tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(
            const variant_bidirectional_iterator<R1, R2, const_>& other
        ) const {
            if (it_.index() == 0 && other.it_.index() == 0) {
                return std::get<0>(it_) == std::get<0>(other.it_);
            } else if (it_.index() == 1 && other.it_.index() == 1) {
                return std::get<1>(it_) == std::get<1>(other.it_);
            }
            return false;
        }
        
        bool operator==(
            const variant_bidirectional_sentinel<R1, R2, const_>& other
        ) const {
            if (it_.index() == 0 && other.it_.index() == 0) {
                return std::get<0>(it_) == std::get<0>(other.it_);
            } else if (it_.index() == 1 && other.it_.index() == 1) {
                return std::get<1>(it_) == std::get<1>(other.it_);
            }
            return false;
        }
    };

    /**
     * View that wraps one of two possible bidirectional range types. Useful for std::views::join'ing range types that aren't the same but
     * contain the same element type.
     *
     * @tparam R1 Wrapped bidirectional range type 1.
     * @tparam R2 Wrapped bidirectional range type 2.
     */
    template<std::ranges::bidirectional_range R1, std::ranges::bidirectional_range R2>
    class variant_bidirectional_view : public std::ranges::view_interface<variant_bidirectional_view<R1, R2>> {
    private:
        std::variant<R1, R2> range;
    public:
        /** Iterator type. */
        using I = variant_bidirectional_iterator<R1, R2, false>;
        /** Iterator sentinel type. */
        using S = variant_bidirectional_sentinel<R1, R2, false>;
        /** Constant iterator type. */
        using IC = variant_bidirectional_iterator<R1, R2, true>;
        /** Constant iterator sentinel type. */
        using SC = variant_bidirectional_sentinel<R1, R2, true>;

        variant_bidirectional_view() requires (std::default_initializable<R1> && std::default_initializable<R2>) = default;
        /**
         * Construct an @ref offbynull::helpers::variant_bidirectional_view::variant_bidirectional_view instance.
         *
         * @param range_ Range to wrap.
         */
        variant_bidirectional_view(R1&& range_) : range { std::move(range_) } {}
        /**
         * Construct an @ref offbynull::helpers::variant_bidirectional_view::variant_bidirectional_view instance.
         *
         * @param range_ Range to wrap.
         */
        variant_bidirectional_view(const R1& range_) : range { range_ } {}
        /**
         * Construct an @ref offbynull::helpers::variant_bidirectional_view::variant_bidirectional_view instance.
         *
         * @param range_ Range to wrap.
         */
        variant_bidirectional_view(R2&& range_) : range { std::move(range_) } {}
        /**
         * Construct an @ref offbynull::helpers::variant_bidirectional_view::variant_bidirectional_view instance.
         *
         * @param range_ Range to wrap.
         */
        variant_bidirectional_view(const R2& range_) : range { range_ } {}

        variant_bidirectional_view(const variant_bidirectional_view<R1, R2> &src) = default;
        variant_bidirectional_view(variant_bidirectional_view<R1, R2> &&src) = default;
        variant_bidirectional_view<R1, R2>& operator=(const variant_bidirectional_view<R1, R2>& other) = default;
        variant_bidirectional_view<R1, R2>& operator=(variant_bidirectional_view<R1, R2>&& other) = default;

        /**
         * Get begin iterator.
         *
         * @return begin iterator.
         */
        I begin() {
            if (std::holds_alternative<R1>(range)) {
                return I { std::in_place_index<0>, std::ranges::begin(std::get<R1>(range)) };
            }
            return I { std::in_place_index<1>, std::ranges::begin(std::get<R2>(range)) };
        }
        /**
         * Get end iterator.
         *
         * @return end iterator.
         */
        S end() {
            if (std::holds_alternative<R1>(range)) {
                return S { std::in_place_index<0>, std::ranges::end(std::get<R1>(range)) };
            }
            return S { std::in_place_index<1>, std::ranges::end(std::get<R2>(range)) };
        }

        /**
         * Get begin iterator.
         *
         * @return begin iterator (const).
         */
        IC begin() const {
            if (std::holds_alternative<R1>(range)) {
                return IC { std::in_place_index<0>, std::ranges::begin(std::as_const(std::get<R1>(range))) };
            }
            return IC { std::in_place_index<1>, std::ranges::begin(std::as_const(std::get<R2>(range))) };
        }
        /**
         * Get end iterator.
         *
         * @return end iterator (const).
         */
        SC end() const {
            if (std::holds_alternative<R1>(range)) {
                return SC { std::in_place_index<0>, std::ranges::end(std::as_const(std::get<R1>(range))) };
            }
            return SC { std::in_place_index<1>, std::ranges::end(std::as_const(std::get<R2>(range))) };
        }
    };
}

#endif //OFFBYNULL_HELPERS_VARIANT_BIDIRECTIONAL_VIEW_H