#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_H

#include <cstddef>
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;

    PACK_STRUCT_START
    template<backtrackable_node N, backtrackable_edge E, weight WEIGHT>
    struct slot {
        N node;
        std::size_t unwalked_parent_cnt;
        E backtracking_edge;
        WEIGHT backtracking_weight;

        slot(N node_, std::size_t unwalked_parent_cnt_)
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

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_H
