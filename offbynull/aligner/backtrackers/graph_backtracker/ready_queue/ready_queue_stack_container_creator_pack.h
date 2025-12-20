#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/unimplemented_ready_queue_container_creator_pack.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack
        ::ready_queue_container_creator_pack;
    using offbynull::utils::static_vector_typer;

    /**
     * @ref offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack::ready_queue_container_creator_pack
     * that allocates its containers on the stack (sometimes escaping to heap if size gets too large).
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam max_queue_elems For a container created via `create_queue_container()`, the maximum number of queue elements allowed on the
     *     stack.
     */
    template<
        bool debug_mode,
        std::size_t max_queue_elems
    >
    struct ready_queue_stack_container_creator_pack {
        /** `create_queue_container()` return type. */
        using QUEUE_CONTAINER_TYPE = typename static_vector_typer<
            debug_mode,
            std::size_t,
            max_queue_elems
        >::type;

        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::ready_queue::unimplemented_ready_queue_container_creator_pack::unimplemented_ready_queue_container_creator_pack::create_queue_container
         */
        QUEUE_CONTAINER_TYPE create_queue_container() const {
            return {};
        }
    };

    static_assert(ready_queue_container_creator_pack<ready_queue_stack_container_creator_pack<true, 10zu>>);
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
