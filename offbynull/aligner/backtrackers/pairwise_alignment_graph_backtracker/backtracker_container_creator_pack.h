#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/ready_queue/ready_queue_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/slot_container/slot_container_container_creator_pack.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtracker_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot_container_container_creator_pack
        ::slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::ready_queue::ready_queue_container_creator_pack
        ::ready_queue_container_creator_pack;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::unqualified_object_type;

    template<
        typename T,
        typename N,
        typename E,
        typename ED,
        typename PARENT_COUNT,
        typename SLOT_INDEX
    >
    concept backtracker_container_creator_pack =
        unqualified_object_type<T>
        && backtrackable_node<N>
        && backtrackable_edge<E>
        && weight<ED>
        && widenable_to_size_t<PARENT_COUNT>
        && widenable_to_size_t<SLOT_INDEX>
        && requires(const T t, std::size_t path_edge_capacity) {
            { t.create_slot_container_container_creator_pack() } -> slot_container_container_creator_pack<N, E, ED, PARENT_COUNT>;
            { t.create_ready_queue_container_creator_pack() } -> ready_queue_container_creator_pack<SLOT_INDEX>;
            { t.create_path_container(path_edge_capacity) } -> random_access_range_of_type<E>;
        };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_CONTAINER_CREATOR_PACK_H
