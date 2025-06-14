#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/ready_queue/ready_queue_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/slot_container/slot_container_stack_container_creator_pack.h"
#include "offbynull/concepts.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtracker_stack_container_creator_pack {
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container
        ::slot_container_stack_container_creator_pack::slot_container_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::ready_queue::ready_queue_stack_container_creator_pack
        ::ready_queue_stack_container_creator_pack;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::utils::static_vector_typer;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        widenable_to_size_t PARENT_COUNT,
        widenable_to_size_t SLOT_INDEX,
        std::size_t grid_down_cnt,
        std::size_t grid_right_cnt,
        std::size_t grid_depth_cnt,
        std::size_t path_edge_capacity
    >
    struct backtracker_stack_container_creator_pack {
        slot_container_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            PARENT_COUNT,
            grid_down_cnt,
            grid_right_cnt,
            grid_depth_cnt
        > create_slot_container_container_creator_pack() const {
            return {};
        }

        ready_queue_stack_container_creator_pack<
            debug_mode,
            SLOT_INDEX,
            grid_down_cnt,
            grid_right_cnt,
            grid_depth_cnt
        > create_ready_queue_container_creator_pack() const {
            return {};
        }

        using PATH_CONTAINER_TYPE = typename static_vector_typer<debug_mode, E, path_edge_capacity>::type;
        PATH_CONTAINER_TYPE create_path_container(std::size_t path_edge_capacity_) const {
            if constexpr (debug_mode) {
                if (path_edge_capacity != path_edge_capacity_) {
                    throw std::runtime_error { "Size mismatch" };
                }
            }
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
