#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H

#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/unimplemented_path_container_container_creator_pack.h"
#include <cstddef>
#include <array>
#include <stdexcept>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::path_container_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;

    /**
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::path_container_container_creator_pack::path_container_container_creator_pack
     * that allocates its containers on the stack.
     *
     * @tparam debug_mode `true` to enable debugging logic, `false` otherwise.
     * @tparam E Graph edge identifier type.
     * @tparam path_edge_capacity Maximum number of elements the container can hold.
     */
    template<
        bool debug_mode,
        backtrackable_edge E,
        std::size_t path_edge_capacity
    >
    struct path_container_stack_container_creator_pack {
        /**
         * @copydoc offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::unimplemented_path_container_container_creator_pack::unimplemented_path_container_container_creator_pack::create_element_container
         */
        std::array<
            element<E>,
            path_edge_capacity
        > create_element_container(std::size_t path_edge_capacity_) const {
            if constexpr (debug_mode) {
                if (path_edge_capacity != path_edge_capacity_) {
                    throw std::runtime_error { "Bad element count" };
                }
            }
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H

