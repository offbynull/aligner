#ifndef OFFBYNULL_ALIGNER_SCORERS_SUBSTITUTION_MAP_SCORER_H
#define OFFBYNULL_ALIGNER_SCORERS_SUBSTITUTION_MAP_SCORER_H

#include <concepts>
#include <optional>
#include <map>
#include <utility>
#include <functional>
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/concepts.h"

/**
 * @ref offbynull::aligner::scorer::scorer::scorer which scores elements using a \code std::map \endcode.
 *
 * @author Kasra Faghihi
 */
namespace offbynull::aligner::scorers::substitution_map_scorer {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::scorer::scorer::scorer;

    /**
     * @ref offbynull::aligner::scorer::scorer::scorer which scores elements using a \code std::map \endcode.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam DOWN_ELEM Type of alignment graph's downward elements.
     * @tparam RIGHT_ELEM Type of alignment graph's rightward elements.
     * @tparam WEIGHT Type of alignment graph's edge weights.
     */
    template<bool debug_mode, typename DOWN_ELEM, typename RIGHT_ELEM, weight WEIGHT>
    requires requires (const DOWN_ELEM down_elem, const RIGHT_ELEM right_elem) {
        { down_elem < down_elem } -> std::same_as<bool>;
        { right_elem < right_elem } -> std::same_as<bool>;
    }
    class substitution_map_scorer {
    private:
        const std::map<std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>>, WEIGHT> data;

    public:
        /**
         * Construct an @ref offbynull::aligner::scorers::substitution_map_scorer::substitution_map_scorer instance.
         *
         * @param data_ Map of scores between element pairs (copied).
         */
        substitution_map_scorer(
            const std::map<std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>>, WEIGHT>& data_
        )
        : data { data_ } {}

        /**
         * Construct an @ref offbynull::aligner::scorers::substitution_map_scorer::substitution_map_scorer instance.
         *
         * @param data_ Map of scores between element pairs (moved).
         */
        substitution_map_scorer(
            std::map<std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>>, WEIGHT>&& data_
        )
        : data { std::move(data_) } {}

        /**
         * Score edge.
         *
         * If no score exists for \code (down_elem, right_elem) \endcode, the behavior is undefined.
         *
         * @param down_elem Downward element.
         * @param right_elem Rightward element.
         * @return Score for edge (edge weight).
         */
        WEIGHT operator()(
            const auto& /*edge*/,
            const std::optional<std::reference_wrapper<const DOWN_ELEM>> down_elem,
            const std::optional<std::reference_wrapper<const RIGHT_ELEM>> right_elem
        ) const {
            if (down_elem.has_value() && right_elem.has_value()) {
                std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>> lookup {
                    { (*down_elem).get() },
                    { (*right_elem).get() }
                };
                return data.at(lookup);
            } else if (down_elem.has_value()) {
                std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>> lookup {
                    { (*down_elem).get() },
                    { std::nullopt }
                };
                return data.at(lookup);
            } else if (right_elem.has_value()) {
                std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>> lookup {
                    { std::nullopt },
                    { (*right_elem).get() }
                };
                return data.at(lookup);
            } else {
                std::pair<std::optional<DOWN_ELEM>, std::optional<RIGHT_ELEM>> lookup {
                    { std::nullopt },
                    { std::nullopt }
                };
                return data.at(lookup);
            }
        }
    };

    static_assert(
        scorer<
            substitution_map_scorer<true, char, char, float>,
            std::pair<int, int>,
            char,
            char,
            float
        >
    );
}

#endif //OFFBYNULL_ALIGNER_SCORERS_SUBSTITUTION_MAP_SCORER_H
