#ifndef OFFBYNULL_ALIGNER_SCORERS_LEVENSHTEIN_SCORER_H
#define OFFBYNULL_ALIGNER_SCORERS_LEVENSHTEIN_SCORER_H

#include <utility>
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/scorers/simple_scorer.h"

/**
 * @ref offbynull::aligner::scorer::scorer::scorer which scores elements using Levenshtein distance (string distance).
 *
 * @author Kasra Faghihi
 */
namespace offbynull::aligner::scorers::levenshtein_scorer {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::scorer::scorer::scorer;
    using offbynull::aligner::scorers::simple_scorer::simple_scorer;

    /**
     * @ref offbynull::aligner::scorer::scorer::scorer which scores elements using Levenshtein distance (string distance).
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam DOWN_ELEM Type of alignment graph's downward elements.
     * @tparam RIGHT_ELEM Type of alignment graph's rightward elements.
     * @tparam WEIGHT Type of alignment graph's edge weights.
     */
    template<bool debug_mode, typename DOWN_ELEM, typename RIGHT_ELEM, weight WEIGHT>
    class levenshtein_scorer : public simple_scorer<debug_mode, DOWN_ELEM, RIGHT_ELEM, WEIGHT> {
    public:
        levenshtein_scorer()
        : simple_scorer<debug_mode, DOWN_ELEM, RIGHT_ELEM, WEIGHT> {
            static_cast<WEIGHT>(-1),
            static_cast<WEIGHT>(0),
            static_cast<WEIGHT>(0),
            static_cast<WEIGHT>(0),
            static_cast<WEIGHT>(0)
        } {}
    };

    static_assert(
        scorer<
            simple_scorer<true, char, char, float>,
            std::pair<int, int>,
            char,
            char,
            float
        >
    );
}

#endif //OFFBYNULL_ALIGNER_SCORERS_LEVENSHTEIN_SCORER_H
