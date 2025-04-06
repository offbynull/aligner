#ifndef OFFBYNULL_ALIGNER_SCORERS_SIMPLE_SCORER_H
#define OFFBYNULL_ALIGNER_SCORERS_SIMPLE_SCORER_H

#include <concepts>
#include <optional>
#include <limits>
#include <functional>
#include <utility>
#include "offbynull/aligner/scorer/scorer.h"
#include "offbynull/aligner/concepts.h"

/**
 * @ref offbynull::aligner::scorer::scorer::scorer which returns a score depending on the presence of an edge's elements and whether those
 * elements match (does not factor in the content of the elements).
 *
 * @author Kasra Faghihi
 */
namespace offbynull::aligner::scorers::simple_scorer {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::scorer::scorer::scorer;

    /**
     * @ref offbynull::aligner::scorer::scorer::scorer which returns a score depending on the presence of an edge's elements and whether
     * those elements match (does not factor in the content of the elements).
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam DOWN_ELEM Type of alignment graph's downward elements.
     * @tparam RIGHT_ELEM Type of alignment graph's rightward elements.
     * @tparam WEIGHT Type of alignment graph's edge weights.
     */
    template<bool debug_mode, typename DOWN_ELEM, typename RIGHT_ELEM, weight WEIGHT>
    requires requires (const DOWN_ELEM down_elem, const RIGHT_ELEM right_elem) {
        { down_elem == right_elem } -> std::same_as<bool>;
    }
    class simple_scorer {
    private:
        const WEIGHT match_weight;
        const WEIGHT mismatch_weight;
        const WEIGHT down_missing_weight;
        const WEIGHT right_missing_weight;
        const WEIGHT both_missing_weight;

    public:
        /**
         * Construct an @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance.
         *
         * @param match_weight_ Score to return when elements match.
         * @param mismatch_weight_ Score to return when elements don't match.
         * @param down_missing_weight_ Score to return when down element is missing.
         * @param right_missing_weight_ Score to return when right element is missing.
         * @param both_missing_weight_ Score to return when both elements are missing.
         */
        simple_scorer(
            WEIGHT match_weight_,
            WEIGHT mismatch_weight_,
            WEIGHT down_missing_weight_,
            WEIGHT right_missing_weight_,
            WEIGHT both_missing_weight_
        )
        : match_weight { match_weight_ }
        , mismatch_weight { mismatch_weight_ }
        , down_missing_weight { down_missing_weight_ }
        , right_missing_weight { right_missing_weight_ }
        , both_missing_weight { both_missing_weight_ } {}

        /**
         * Create an @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance explicitly targeting
         * substitution edges (as opposed to indel or freeride).
         *
         * @param match_weight_ Score to return when elements match.
         * @param mismatch_weight_ Score to return when elements don't match.
         * @return New @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance.
         */
        static simple_scorer<debug_mode, DOWN_ELEM, RIGHT_ELEM, WEIGHT> create_substitution(
            WEIGHT match_weight_,
            WEIGHT mismatch_weight_
        ) {
            constexpr bool nan_available { std::numeric_limits<WEIGHT>::has_quiet_NaN };
            constexpr WEIGHT missing_val { nan_available ? std::numeric_limits<WEIGHT>::quiet_NaN() : std::numeric_limits<WEIGHT>::max() };
            return {
                match_weight_,
                mismatch_weight_,
                missing_val,
                missing_val,
                missing_val
            };
        }

        /**
         * Create an @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance explicitly targeting
         * indel edges (as opposed to substitution or freeride).
         *
         * @param gap_weight_ Score to return when either down or right element is missing.
         * @return New @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance.
         */
        static simple_scorer<debug_mode, DOWN_ELEM, RIGHT_ELEM, WEIGHT> create_gap(
            WEIGHT gap_weight_
        ) {
            constexpr bool nan_available { std::numeric_limits<WEIGHT>::has_quiet_NaN };
            constexpr WEIGHT missing_val { nan_available ? std::numeric_limits<WEIGHT>::quiet_NaN() : std::numeric_limits<WEIGHT>::max() };
            return {
                missing_val,
                missing_val,
                gap_weight_,
                gap_weight_,
                missing_val
            };
        }

        /**
         * Create an @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance explicitly targeting
         * indel edges (as opposed to substitution or freeride), with different scoring based on if the indel is downward or rightward.
         *
         * @param down_gap_weight_ Score to return when down element is missing.
         * @param right_gap_weight_ Score to return when right element is missing.
         * @return New @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance.
         */
        static simple_scorer<debug_mode, DOWN_ELEM, RIGHT_ELEM, WEIGHT> create_gap_asymmetric(
            WEIGHT down_gap_weight_,
            WEIGHT right_gap_weight_
        ) {
            constexpr bool nan_available { std::numeric_limits<WEIGHT>::has_quiet_NaN };
            constexpr WEIGHT missing_val { nan_available ? std::numeric_limits<WEIGHT>::quiet_NaN() : std::numeric_limits<WEIGHT>::max() };
            return {
                missing_val,
                missing_val,
                down_gap_weight_,
                right_gap_weight_,
                missing_val
            };
        }

        /**
         * Create an @ref offbynull::aligner::scorers::simple_scorer::simple_scorer instance explicitly targeting
         * freeride edges (as opposed to substitution or idel).
         *
         * @param freeride_weight_ Score to return when both elements are missing.
         * @return New @ref offbynull::aligner::scorers::simple_scorer::simple_scorer::simple_scorer instance.
         */
        static simple_scorer<debug_mode, DOWN_ELEM, RIGHT_ELEM, WEIGHT> create_freeride(
            WEIGHT freeride_weight_ = {}
        ) {
            constexpr bool nan_available { std::numeric_limits<WEIGHT>::has_quiet_NaN };
            constexpr WEIGHT missing_val { nan_available ? std::numeric_limits<WEIGHT>::quiet_NaN() : std::numeric_limits<WEIGHT>::max() };
            return {
                missing_val,
                missing_val,
                missing_val,
                missing_val,
                freeride_weight_
            };
        }

        /**
         * Score edge.
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
                if ((*down_elem).get() == (*right_elem).get()) {
                    return match_weight;
                } else {
                    return mismatch_weight;
                }
            } else if (!down_elem.has_value() && right_elem.has_value()) {
                return down_missing_weight;
            } else if (down_elem.has_value() && !right_elem.has_value()) {
                return right_missing_weight;
            } else {
                return both_missing_weight;
            }
        }
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

#endif //OFFBYNULL_ALIGNER_SCORERS_SIMPLE_SCORER_H
