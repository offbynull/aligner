#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_H

#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/graph/pairwise_alignment_graph.h"
#include "offbynull/utils.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot {
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;

    // Because this struct is heavily templated, it's impossible to figure out how to organize member variables to reduce padding. One
    // option may be to ask the compiler to specifically pack the struct, but this may cause performance problems and/or bugs to happen
    // (depending on the architecture you're compiling for)?
    PACK_STRUCT_START
    template<
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        widenable_to_size_t PARENT_COUNT
    >
    struct slot {
        N node;
        PARENT_COUNT unwalked_parent_cnt;
        E backtracking_edge;
        ED backtracking_weight;

        slot(N node_, PARENT_COUNT unwalked_parent_cnt_)
        : node { node_ }
        , unwalked_parent_cnt { unwalked_parent_cnt_ }
        , backtracking_edge {}
        , backtracking_weight {} {}

        slot()
        : node {}
        , unwalked_parent_cnt {}
        , backtracking_edge {}
        , backtracking_weight {} {}
    }
    PACK_STRUCT_STOP;
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_H
