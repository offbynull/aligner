#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/ready_queue/ready_queue_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot_container_stack_container_creator_pack.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::backtracker_stack_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_stack_container_creator_pack
        ::slot_container_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack
        ::ready_queue_stack_container_creator_pack;
    using offbynull::utils::static_vector_typer;

    /**
     * @ref offbynull::aligner::backtrackers::graph_backtracker::backtracker_container_creator_pack::backtracker_container_creator_pack
     * that allocates its containers on the heap.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam N Graph node identifier type.
     * @tparam E Graph edge identifier type.
     * @tparam WEIGHT Graph edge's weight type.
     * @tparam max_slot_container_elems For a container created via
     *     `create_slot_container_container_creator_pack().create_slot_container()`, the maximum number of slots allowed on the stack.
     * @tparam max_ready_queue_elems For a container created via `create_ready_queue_container_creator_pack().create_queue_container()`, the
     *     maximum number of queue elements allowed on the stack.
     * @tparam max_path_elems For a container created via `create_path_container()`, the maximum number of elements allowed on the stack.
     */
    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT,
        std::size_t max_slot_container_elems,
        std::size_t max_ready_queue_elems,
        std::size_t max_path_elems
    >
    struct backtracker_stack_container_creator_pack {
        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::unimplemented_backtracker_container_creator_pack::unimplemented_backtracker_container_creator_pack::create_slot_container_container_creator_pack
         */
        slot_container_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            WEIGHT,
            max_slot_container_elems
        > create_slot_container_container_creator_pack() const {
            return {};
        }

        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::unimplemented_backtracker_container_creator_pack::unimplemented_backtracker_container_creator_pack::create_ready_queue_container_creator_pack
         */
        ready_queue_stack_container_creator_pack<
            debug_mode,
            max_ready_queue_elems
        > create_ready_queue_container_creator_pack() const {
            return {};
        }

        /** `create_path_container()` return type. */
        using PATH_CONTAINER_TYPE = typename static_vector_typer<
            debug_mode,
            E,
            max_path_elems
        >::type;

        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::unimplemented_backtracker_container_creator_pack::unimplemented_backtracker_container_creator_pack::create_path_container
         */
        PATH_CONTAINER_TYPE create_path_container() const {
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
