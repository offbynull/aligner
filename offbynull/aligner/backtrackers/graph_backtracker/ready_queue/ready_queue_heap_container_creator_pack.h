#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <vector>
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack
        ::ready_queue_container_creator_pack;


    template<
        bool debug_mode
    >
    struct ready_queue_heap_container_creator_pack {
        std::vector<std::size_t> create_queue_container() const {
            return {};
        }
    };

    static_assert(ready_queue_container_creator_pack<ready_queue_heap_container_creator_pack<true>>);
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H
