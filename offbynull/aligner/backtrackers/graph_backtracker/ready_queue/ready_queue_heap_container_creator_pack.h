#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <vector>
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/unimplemented_ready_queue_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_heap_container_creator_pack {
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack
        ::ready_queue_container_creator_pack;


    /**
     * @ref offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack::ready_queue_container_creator_pack
     * that allocates its containers on the heap.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     */
    template<
        bool debug_mode
    >
    struct ready_queue_heap_container_creator_pack {
        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::ready_queue::unimplemented_ready_queue_container_creator_pack::unimplemented_ready_queue_container_creator_pack::create_queue_container
         */
        std::vector<std::size_t> create_queue_container() const {
            return {};
        }
    };

    static_assert(ready_queue_container_creator_pack<ready_queue_heap_container_creator_pack<true>>);
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_HEAP_CONTAINER_CREATOR_PACK_H
