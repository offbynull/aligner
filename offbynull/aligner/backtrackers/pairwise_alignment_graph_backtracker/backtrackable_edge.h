#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKABLE_EDGE_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKABLE_EDGE_H

#include <concepts>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_edge {
    using offbynull::concepts::unqualified_object_type;

    template<typename E>
    concept backtrackable_edge = unqualified_object_type<E> && std::regular<E>;
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKABLE_EDGE_H
