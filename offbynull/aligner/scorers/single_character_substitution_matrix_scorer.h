#ifndef OFFBYNULL_ALIGNER_SCORERS_PRINTABLE_SINGLE_CHARACTER_SUBSTITUTION_MATRIX_SCORER_H
#define OFFBYNULL_ALIGNER_SCORERS_PRINTABLE_SINGLE_CHARACTER_SUBSTITUTION_MATRIX_SCORER_H

#include <optional>
#include <array>
#include <algorithm>
#include <sstream>
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/concepts.h"

namespace offbynull::aligner::scorers::single_character_substitution_matrix_scorer {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::scorer::scorer::scorer;

    template<weight WEIGHT, std::size_t ALPHABET_SIZE, bool error_check = true>
    class single_character_substitution_matrix_scorer {
    private:
        static_assert(ALPHABET_SIZE <= 255zu, "Alphabet greater than 255 symbols");

        const std::array<char, ALPHABET_SIZE> alphabet;
        const std::array<WEIGHT, ALPHABET_SIZE * ALPHABET_SIZE> weights;
        const char char_for_missing;

        static std::string_view trim_whitespace(const std::string_view& text) {
            auto start {
                std::find_if_not(text.begin(), text.end(), [](auto ch) { return ch == ' ' || ch == '\n'; })
            };
            auto end {
                std::find_if_not(text.rbegin(), text.rend(), [](auto ch) { return ch == ' ' || ch == '\n'; }).base()
            };
            if (start < end) {
                return std::string_view(&*start, std::distance(start, end));
            }
            return std::string_view {};
        }

        static char extract_symbol(const auto& element_view) {
            auto element_view_it { element_view.begin() };
            if constexpr (error_check) {
                if (element_view_it == element_view.end()) {
                    throw std::runtime_error("Expected 1 character for each alphabet item -- empty");
                }
            }
            char ret { *element_view_it };
            if constexpr (error_check) {
                ++element_view_it;
                if (element_view_it != element_view.end()) {
                    throw std::runtime_error("Expected 1 character for each alphabet item -- multiple");
                }
            }
            return ret;
        }

        static std::array<char, ALPHABET_SIZE> extract_sorted_alphabet(const std::string_view& text) {
            // trim leading/trailing whitespace
            std::string_view text_trimmed { trim_whitespace(text) };
            // split test line-by-line
            auto lines { std::views::split(text_trimmed, '\n') };
            auto lines_it { lines.begin() };
            // pull out header and split on space
            auto header_line { *lines_it };
            auto header_line_split {
                std::views::split(header_line, ' ')
                | std::ranges::views::filter([](const auto& s) {
                    return s.begin() != s.end();
                })
            };
            // set alphabet
            if constexpr (error_check) {
                auto size { std::ranges::distance(header_line_split) };
                if (size != ALPHABET_SIZE) {
                    throw std::runtime_error("Unexpected number of characters");
                }
            }
            std::array<char, ALPHABET_SIZE> ret {};
            auto header_line_split_it { header_line_split.begin() };
            for (std::size_t i { 0zu }; i < ALPHABET_SIZE; i++) {
                ret[i] = extract_symbol(*header_line_split_it);
                ++header_line_split_it;
            }
            std::ranges::sort(ret.begin(), ret.end());
            return ret;
        }

        static char extract_last_char_in_header(const std::string_view& text) {
            // trim leading/trailing whitespace
            std::string_view text_trimmed { trim_whitespace(text) };
            // split test line-by-line
            auto lines { std::views::split(text_trimmed, '\n') };
            auto lines_it { lines.begin() };
            // pull out header and split on space
            auto header_line { *lines_it };
            auto header_line_split {
                std::views::split(header_line, ' ')
                | std::ranges::views::filter([](const auto& s) {
                    return s.begin() != s.end();
                })
            };
            // walk to last -- assume last char is the char_for_missing char
            char char_for_missing_char {};
            for (auto col_view : header_line_split) {
                char_for_missing_char = extract_symbol(col_view);
            }
            return char_for_missing_char;
        }

        static std::size_t to_weights_idx(
            const std::array<char, ALPHABET_SIZE> sorted_alphabet,
            const char down_elem,
            const char right_elem
        ) {
            auto down_it { std::lower_bound(sorted_alphabet.begin(), sorted_alphabet.end(), down_elem) };
            if constexpr (error_check) {
                if (*down_it != down_elem) {
                    throw std::runtime_error("Not found");
                }
            }
            auto right_it { std::lower_bound(sorted_alphabet.begin(), sorted_alphabet.end(), right_elem) };
            if constexpr (error_check) {
                if (*right_it != right_elem) {
                    throw std::runtime_error("Not found");
                }
            }
            std::size_t down_idx { down_it - sorted_alphabet.begin() };
            std::size_t right_idx { right_it - sorted_alphabet.begin() };
            return down_idx * ALPHABET_SIZE + right_idx;
        }

        static std::array<WEIGHT, ALPHABET_SIZE * ALPHABET_SIZE> extract_sorted_weights(const std::string_view& text) {
            const std::array<char, ALPHABET_SIZE> sorted_alphabet { std::move(extract_sorted_alphabet(text)) };
            // trim leading/trailing whitespace
            std::string_view text_trimmed { trim_whitespace(text) };
            // split test line-by-line
            auto lines { std::views::split(text_trimmed, '\n') };
            auto lines_it { lines.begin() };
            // pull out header and split on space
            auto header_line { *lines_it };
            auto header_line_split {
                std::views::split(header_line, ' ')
                | std::ranges::views::filter([](const auto& s) {
                    return s.begin() != s.end();
                })
            };
            ++lines_it;
            // pull out rows
            std::array<WEIGHT, ALPHABET_SIZE * ALPHABET_SIZE> ret {};
            while (lines_it != lines.end()) {
                auto row_line { *lines_it };
                ++lines_it;
                // split row
                auto row_line_split {
                    std::views::split(row_line, ' ')
                    | std::ranges::views::filter([](const auto& s) {
                        return s.begin() != s.end();
                    })
                };
                auto row_line_split_it { row_line_split.begin() };
                // pull right value
                auto down_elem_view { *row_line_split_it };
                char down_elem { extract_symbol(down_elem_view) };
                ++row_line_split_it;
                auto header_line_split_it { header_line_split.begin() };
                while (header_line_split_it != header_line_split.end()) {
                    // pull down value
                    auto right_elem_view { *header_line_split_it };
                    char right_elem { extract_symbol(right_elem_view) };
                    ++header_line_split_it;
                    // pull weight
                    auto weight_str { *row_line_split_it };
                    ++row_line_split_it;
                    WEIGHT weight {};
                    bool convert_success { std::istringstream { weight_str.data() } >> weight };
                    if constexpr (error_check) {
                        if (!convert_success) {
                            throw std::runtime_error("Failed to convert string to numeric");
                        }
                    }
                    // insert weight
                    std::size_t ret_idx { to_weights_idx(sorted_alphabet, down_elem, right_elem) };
                    ret[ret_idx] = weight;
                }
            }
            return ret;
        }

    public:
        single_character_substitution_matrix_scorer(
            const std::string_view& text_table
        )
        : alphabet { std::move(extract_sorted_alphabet(text_table)) }
        , weights { std::move(extract_sorted_weights(text_table)) }
        , char_for_missing { extract_last_char_in_header(text_table) } {}

        single_character_substitution_matrix_scorer(
            const std::string_view& text_table,
            const char
        )
        : alphabet { std::move(extract_sorted_alphabet(text_table)) }
        , weights { std::move(extract_sorted_weights(text_table)) }
        , char_for_missing { extract_last_char_in_header(text_table) } {}

        WEIGHT operator()(
            const auto& edge,
            const std::optional<std::reference_wrapper<const char>> down_elem,
            const std::optional<std::reference_wrapper<const char>> right_elem
        ) const {
            char down_elem_ch { down_elem.has_value() ? (*down_elem).get() : char_for_missing };
            char right_elem_ch { right_elem.has_value() ? (*right_elem).get() : char_for_missing };
            std::size_t weights_idx { to_weights_idx(alphabet, down_elem_ch, right_elem_ch) };
            return weights[weights_idx];
        }
    };

    static_assert(
        scorer<
            single_character_substitution_matrix_scorer<float, 95zu>,
            std::pair<int, int>,
            char,
            char,
            float
        >
    );
}

#endif //OFFBYNULL_ALIGNER_SCORERS_PRINTABLE_SINGLE_CHARACTER_SUBSTITUTION_MATRIX_SCORER_H
