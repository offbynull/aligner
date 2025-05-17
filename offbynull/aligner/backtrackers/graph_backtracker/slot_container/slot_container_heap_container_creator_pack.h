#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H

#include <vector>
#include "offbynull/aligner/backtrackers/graph_backtracker/concepts.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot_container_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_heap_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::concepts::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::concepts::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_container_creator_pack
        ::slot_container_container_creator_pack;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT
    >
    struct slot_container_heap_container_creator_pack {
        std::vector<
            slot<N, E, WEIGHT>
        > create_slot_container(auto begin, auto end) const {
            return std::vector<slot<N, E, WEIGHT>>(begin, end);
        }
    };

    static_assert(
        slot_container_container_creator_pack<slot_container_heap_container_creator_pack<true, int, int, float>, int, int, float>
    );
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_HEAP_CONTAINER_CREATOR_PACK_H
