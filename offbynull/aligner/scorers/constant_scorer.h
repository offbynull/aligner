#ifndef OFFBYNULL_ALIGNER_SCORERS_CONSTANT_SCORER_H
#define OFFBYNULL_ALIGNER_SCORERS_CONSTANT_SCORER_H

#include <utility>
#include <optional>
#include <functional>
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/concepts.h"

/**
 * @ref offbynull::aligner::scorer::scorer::scorer which returns a constant score, regardless of edge.
 *
 * @author Kasra Faghihi
 */
namespace offbynull::aligner::scorers::constant_scorer {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::scorer::scorer::scorer;

    /**
     * Constant @ref offbynull::aligner::scorer::scorer::scorer, returning the same score regardless of edge.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam DOWN_ELEM Type of alignment graph's downward elements.
     * @tparam RIGHT_ELEM Type of alignment graph's rightward elements.
     * @tparam WEIGHT Type of alignment graph's edge weights.
     */
    template<bool debug_mode, typename DOWN_ELEM, typename RIGHT_ELEM, weight WEIGHT>
    class constant_scorer {
    private:
        const WEIGHT weight;

    public:
        /**
         * Construct an @ref offbynull::aligner::scorers::constant_scorer::constant_scorer::constant_scorer instance.
         *
         * @param weight_ Score to return on each invocation.
         */
        constant_scorer(WEIGHT weight_)
        : weight { weight_ } {}

        /**
         * Score edge.
         *
         * @return Score for edge (edge weight).
         */
        WEIGHT operator()(
            const auto& /*edge*/,
            const std::optional<std::reference_wrapper<const DOWN_ELEM>> /*down_elem*/,
            const std::optional<std::reference_wrapper<const RIGHT_ELEM>> /*right_elem*/
        ) const {
            return weight;
        }
    };

    static_assert(
        scorer<
            constant_scorer<true, char, char, float>,
            std::pair<int, int>,
            char,
            char,
            float
        >
    );
}

#endif //OFFBYNULL_ALIGNER_SCORERS_CONSTANT_SCORER_H
