#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKABLE_NODE_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKABLE_NODE_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node {
    using offbynull::concepts::unqualified_object_type;

    template<typename N>
    concept backtrackable_node =
        unqualified_object_type<N>
        && std::regular<N>
        && requires(N n) {
            { n < n } -> std::same_as<bool>;
        };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKABLE_NODE_H
