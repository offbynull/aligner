#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_ELEMENT_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_ELEMENT_H

#include "offbynull/utils.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;

    PACK_STRUCT_START
    template<backtrackable_edge E>
    struct element {
        element<E>* prev {};
        element<E>* next {};
        E backtracking_edge {};

        element() = default;

        bool operator==(const element &) const = default;
    }
    PACK_STRUCT_STOP;
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_ELEMENT_H
