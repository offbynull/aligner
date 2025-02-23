#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_CONCEPTS_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_CONCEPTS_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::concepts {
    using offbynull::concepts::unqualified_object_type;



    template<typename N>
    concept backtrackable_node =
        unqualified_object_type<N>
        && std::regular<N>
        && requires(N n) {
            { n < n } -> std::same_as<bool>;
        };

    template<typename E>
    concept backtrackable_edge =
        unqualified_object_type<E>
        && std::regular<E>;
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_CONCEPTS_H
