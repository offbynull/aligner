#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_EDGE_WEIGHT_ACCESSOR_WITHOUT_EXPLICIT_WEIGHT_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_EDGE_WEIGHT_ACCESSOR_WITHOUT_EXPLICIT_WEIGHT_H

#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::edge_weight_accessor_without_explicit_weight {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;

    template<
        typename T,
        typename E
    >
    concept edge_weight_accessor_without_explicit_weight =
        // leave out unqualified_value_type<T> because it won't pass if T is a function pointer? or maybe it will?
        backtrackable_edge<E>
        && requires(const T t, const E& e) {
            { t(e) } -> weight;
        };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_EDGE_WEIGHT_ACCESSOR_WITHOUT_EXPLICIT_WEIGHT_H
