#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <boost/container/small_vector.hpp>
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/unimplemented_ready_queue_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack
        ::ready_queue_container_creator_pack;

    /**
     * @ref offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack::ready_queue_container_creator_pack
     * that allocates its containers on the stack (sometimes escaping to heap if size gets too large).
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam heap_escape_size For a container created via `create_queue_container()`, the maximum number of queue elements allowed on the
     *     stack before escaping to heap.
     */
    template<
        bool debug_mode,
        std::size_t heap_escape_size = 100zu
    >
    struct ready_queue_stack_container_creator_pack {
        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::ready_queue::unimplemented_ready_queue_container_creator_pack::unimplemented_ready_queue_container_creator_pack::create_queue_container
         */
        boost::container::small_vector<
            std::size_t,
            heap_escape_size
        > create_queue_container() const {
            return {};
        }
    };

    static_assert(ready_queue_container_creator_pack<ready_queue_stack_container_creator_pack<true>>);
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
