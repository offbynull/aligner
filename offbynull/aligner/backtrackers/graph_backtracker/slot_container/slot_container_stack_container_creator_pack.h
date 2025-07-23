#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <boost/container/small_vector.hpp>
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/slot_container_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/graph_backtracker/slot_container/unimplemented_slot_container_container_creator_pack.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_stack_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot::slot;
    using offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_container_creator_pack
        ::slot_container_container_creator_pack;

    /**
     * @ref offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_container_creator_pack::slot_container_container_creator_pack
     * that allocates its containers on the heap.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam N Graph node type.
     * @tparam E Graph edge type.
     * @tparam WEIGHT Graph edge's weight type.
     * @tparam heap_escape_size For a container created via `create_slot_container()`, the maximum number of slots allowed on the stack
     *     before escaping to heap.
     */
    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight WEIGHT,
        std::size_t heap_escape_size = 100zu
    >
    struct slot_container_stack_container_creator_pack {
        /**
         * @copydoc offbynull::aligner::backtrackers::graph_backtracker::slot_container::unimplemented_slot_container_container_creator_pack::unimplemented_slot_container_container_creator_pack::create_slot_container
         */
        boost::container::small_vector<
            slot<N, E, WEIGHT>,
            heap_escape_size
        > create_slot_container(auto begin, auto end) const {
            return boost::container::small_vector<slot<N, E, WEIGHT>, heap_escape_size>(begin, end);
        }
    };

    static_assert(
        slot_container_container_creator_pack<slot_container_stack_container_creator_pack<true, int, int, float>, int, int, float>
    );
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
