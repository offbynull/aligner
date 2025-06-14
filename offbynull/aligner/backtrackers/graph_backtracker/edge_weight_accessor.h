#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_EDGE_WEIGHT_ACCESSOR_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_EDGE_WEIGHT_ACCESSOR_H

#include <concepts>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::edge_weight_accessor {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;

    template<
        typename T,
        typename E,
        typename WEIGHT
    >
    concept edge_weight_accessor =
        // leave out unqualified_value_type<T> because it won't pass if T is a function pointer? or maybe it will?
        backtrackable_edge<E>
        && weight<WEIGHT>
        && requires(const T t, const E& e) {
            { t(e) } -> std::same_as<WEIGHT>;
        };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_EDGE_WEIGHT_ACCESSOR_H
