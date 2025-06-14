#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_CONTAINER_CREATOR_PACK_H

#include <vector>
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot_container_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::unqualified_object_type;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot;

    template<
        typename T,
        typename N,
        typename E,
        typename WEIGHT
    >
    concept slot_container_container_creator_pack =
        unqualified_object_type<T>
        && backtrackable_node<N>
        && backtrackable_edge<E>
        && weight<WEIGHT>
        && requires(const T t, std::vector<slot<N, E, WEIGHT>> fake_range) {
            { t.create_slot_container(fake_range.begin(), fake_range.end()) } -> random_access_range_of_type<slot<N, E, WEIGHT>>;
        };

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
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_CONTAINER_CREATOR_PACK_H
