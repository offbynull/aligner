#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack {
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::unqualified_object_type;


    template<
        typename T
    >
    concept ready_queue_container_creator_pack =
        unqualified_object_type<T>
        && requires(const T t) {
            { t.create_queue_container() } -> random_access_range_of_type<std::size_t>;
        };
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_CONTAINER_CREATOR_PACK_H
