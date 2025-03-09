#ifndef OFFBYNULL_ALIGNER_SCORER_SCORER_H
#define OFFBYNULL_ALIGNER_SCORER_SCORER_H

#include <concepts>
#include <optional>
#include <functional>
#include <type_traits>
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"

/**
 * Scorer interface.
 *
 * @author Kasra Faghihi
 */
namespace offbynull::aligner::scorer::scorer {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::concepts::unqualified_object_type;

    /**
     * Concept that's satisfied if `T` has the traits of a scorer. A scorer is a callable that scores an alignment graph's edge.
     *
     * @tparam T Type to check.
     * @tparam EDGE Type of alignment graph's edge identifiers.
     * @tparam DOWN_ELEM Type of alignment graph's downward elements.
     * @tparam RIGHT_ELEM Type of alignment graph's rightward elements.
     * @tparam WEIGHT Type of alignment graph's edge weights.
     */
    template<typename T, typename EDGE, typename DOWN_ELEM, typename RIGHT_ELEM, typename WEIGHT>
    concept scorer =
        unqualified_object_type<T>
        && requires(
            T t,
            const EDGE& edge,
            const std::optional<std::reference_wrapper<const DOWN_ELEM>> down_elem_ref_opt,
            const std::optional<std::reference_wrapper<const RIGHT_ELEM>> right_elem_ref_opt
        ) {
            { t(edge, down_elem_ref_opt, right_elem_ref_opt) } -> std::same_as<WEIGHT>;
        }
        && std::is_same_v<std::remove_cvref_t<DOWN_ELEM>, DOWN_ELEM>  // make sure no refs / cv-qualifiers
        && std::is_same_v<std::remove_cvref_t<RIGHT_ELEM>, RIGHT_ELEM>  // make sure no refs / cv-qualifiers
        && weight<WEIGHT>;

    /**
     * Equivalent to @ref offbynull::aligner::scorer::scorer::scorer, but doesn't enforce an exact type for the callable's return value.
     *
     * @tparam T Type to check.
     * @tparam EDGE Type of alignment graph's edge identifiers.
     * @tparam DOWN_ELEM Type of alignment graph's downward elements.
     * @tparam RIGHT_ELEM Type of alignment graph's rightward elements.
     */
    template<typename T, typename EDGE, typename DOWN_ELEM, typename RIGHT_ELEM>
    concept scorer_without_explicit_weight =
        unqualified_object_type<T>
        && requires(
            T t,
            const EDGE& edge,
            const std::optional<std::reference_wrapper<const DOWN_ELEM>> down_elem_ref_opt,
            const std::optional<std::reference_wrapper<const RIGHT_ELEM>> right_elem_ref_opt
        ) {
            { t(edge, down_elem_ref_opt, right_elem_ref_opt) } -> weight;
        }
        && std::is_same_v<std::remove_cvref_t<DOWN_ELEM>, DOWN_ELEM>  // make sure no refs / cv-qualifiers
        && std::is_same_v<std::remove_cvref_t<RIGHT_ELEM>, RIGHT_ELEM>;  // make sure no refs / cv-qualifiers
}

#endif //OFFBYNULL_ALIGNER_SCORER_SCORER_H
