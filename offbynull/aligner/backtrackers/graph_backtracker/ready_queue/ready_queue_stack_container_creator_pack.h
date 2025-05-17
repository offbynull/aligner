#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_READY_QUEUE_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <boost/container/small_vector.hpp>

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::graph_backtracker::ready_queue::ready_queue_container_creator_pack
        ::ready_queue_container_creator_pack;

    template<
        bool debug_mode,
        std::size_t heap_escape_size = 100zu
    >
    struct ready_queue_stack_container_creator_pack {
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
