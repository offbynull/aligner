#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::ready_queue::ready_queue_container_creator_pack {
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::unqualified_object_type;

    template<
        typename T,
        typename SLOT_INDEX
    >
    concept ready_queue_container_creator_pack =
        unqualified_object_type<T>
        && widenable_to_size_t<SLOT_INDEX>
        && requires(
            const T t,
            std::size_t grid_down_cnt,
            std::size_t grid_right_cnt,
            std::size_t grid_depth_cnt
        ) {
            { t.create_queue_container(grid_down_cnt, grid_right_cnt, grid_depth_cnt) } -> random_access_range_of_type<SLOT_INDEX>;
        };
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_CONTAINER_CREATOR_PACK_H
