#ifndef OFFBYNULL_ALIGNER_ALIGNER_UTILS_H
#define OFFBYNULL_ALIGNER_ALIGNER_UTILS_H

#include "offbynull/concepts.h"
#include "offbynull/utils.h"
#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/scorers/widening_scorer.h"
#include <cstdint>
#include <cstddef>
#include <variant>
#include <type_traits>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>

namespace offbynull::aligner::aligner_utils {
    using offbynull::utils::narrower_numeric;
    using offbynull::utils::wider_numeric;
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::scorer::scorer::scorer;
    using offbynull::aligner::scorer::scorer::scorer_without_explicit_weight_or_seq_index;
    using offbynull::aligner::scorers::widening_scorer::widening_scorer;

    struct uint8_t_marker {};
    struct uint16_t_marker {};
    struct uint32_t_marker {};
    struct uint64_t_marker {};
    struct size_t_marker {};
    struct float_marker {};
    struct double_marker {};

    template<typename T> struct marker_to_type;
    template<> struct marker_to_type<uint8_t_marker> { using type = std::uint8_t; };
    template<> struct marker_to_type<uint16_t_marker> { using type = std::uint16_t; };
    template<> struct marker_to_type<uint32_t_marker> { using type = std::uint32_t; };
    template<> struct marker_to_type<uint64_t_marker> { using type = std::uint64_t; };
    template<> struct marker_to_type<size_t_marker> { using type = std::size_t; };
    template<> struct marker_to_type<float_marker> { using type = float; };
    template<> struct marker_to_type<double_marker> { using type = double; };

    using index_marker_onehot = std::variant<uint8_t_marker, uint16_t_marker, uint32_t_marker, uint64_t_marker, size_t_marker>;
    using weight_marker_onehot = std::variant<uint8_t_marker, uint16_t_marker, uint32_t_marker, uint64_t_marker, size_t_marker,
        float_marker, double_marker>;

    template<bool debug_mode>
    index_marker_onehot find_narrowest_index_type(
        const sequence auto& down,
        const sequence auto& right,
        const scorer_without_explicit_weight_or_seq_index<
            std::remove_cvref_t<decltype(down[0])>,
            std::remove_cvref_t<decltype(right[0])>
        > auto& substitution_scorer,
        const scorer_without_explicit_weight_or_seq_index<
            std::remove_cvref_t<decltype(down[0])>,
            std::remove_cvref_t<decltype(right[0])>
        > auto& gap_scorer
    ) {
        if constexpr (debug_mode) {
            if (std::max(down.size(), right.size()) == std::numeric_limits<std::size_t>::max()) {
                throw std::runtime_error { "Sequence too large" };  // Needed because of +1 below - no reasonable way this happens?
            }
        }
        std::size_t index_max { std::max(down.size(), right.size()) + 1zu };
        if constexpr (debug_mode) {
            using SUBSTITUTION_SCORER = std::remove_cvref_t<decltype(substitution_scorer)>;
            using GAP_SCORER = std::remove_cvref_t<decltype(gap_scorer)>;
            if (index_max > std::numeric_limits<typename SUBSTITUTION_SCORER::SEQ_INDEX>::max()
                || index_max > std::numeric_limits<typename GAP_SCORER::SEQ_INDEX>::max()) {
                throw std::runtime_error { "Scorer's sequence index type too narrow to support a sequence this large" };
            }
        }
        if (index_max <= std::numeric_limits<std::uint8_t>::max()) {
            return { uint8_t_marker {} };
        } else if (index_max <= std::numeric_limits<std::uint16_t>::max()) {
            return { uint16_t_marker {} };
        } else if (index_max <= std::numeric_limits<std::uint32_t>::max()) {
            return { uint32_t_marker {} };
        } else if (index_max <= std::numeric_limits<std::uint64_t>::max()) {
            return { uint64_t_marker {} };
        } else if (index_max <= std::numeric_limits<std::size_t>::max()) {
            return { size_t_marker {} };
        }
        if constexpr (debug_mode) {
            throw std::runtime_error { "Unable to find type large enough to support index" };
        } else {
            std::unreachable();
        }
    }

    template<bool debug_mode>
    weight_marker_onehot find_widest_weight_type(
        const sequence auto& down,
        const sequence auto& right,
        const scorer_without_explicit_weight_or_seq_index<
            std::remove_cvref_t<decltype(down[0])>,
            std::remove_cvref_t<decltype(right[0])>
        > auto& substitution_scorer,
        const scorer_without_explicit_weight_or_seq_index<
            std::remove_cvref_t<decltype(down[0])>,
            std::remove_cvref_t<decltype(right[0])>
        > auto& gap_scorer
    ) {
        using WEIGHT = typename wider_numeric<
            typename std::remove_cvref_t<decltype(substitution_scorer)>::WEIGHT,
            typename std::remove_cvref_t<decltype(gap_scorer)>::WEIGHT
        >::type;
        if constexpr (std::is_same_v<WEIGHT, std::uint8_t>) {
            return { uint8_t_marker {} };
        } else if constexpr (std::is_same_v<WEIGHT, std::uint16_t>) {
            return { uint16_t_marker {} };
        } else if constexpr (std::is_same_v<WEIGHT, std::uint32_t>) {
            return { uint32_t_marker {} };
        } else if constexpr (std::is_same_v<WEIGHT, std::uint64_t>) {
            return { uint64_t_marker {} };
        } else if constexpr (std::is_same_v<WEIGHT, std::size_t>) {
            return { size_t_marker {} };
        } else if constexpr (std::is_same_v<WEIGHT, float>) {
            return { float_marker {} };
        } else if constexpr (std::is_same_v<WEIGHT, double>) {
            return { double_marker {} };
        }
        if constexpr (debug_mode) {
            throw std::runtime_error { "Unable to find widest type to support weight" };
        } else {
            std::unreachable();
        }
    }
}

#endif //OFFBYNULL_ALIGNER_ALIGNER_UTILS_H
