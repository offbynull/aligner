#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/path_container.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_segmenter/resident_segmenter_stack_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/sliced_subdivider/sliced_subdivider_stack_container_creator_pack.h"
#include "offbynull/aligner/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtracker_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::concepts::weight;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_segmenter
        ::resident_segmenter_stack_container_creator_pack::resident_segmenter_stack_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::sliced_subdivider
        ::sliced_subdivider_stack_container_creator_pack::sliced_subdivider_stack_container_creator_pack;
    using offbynull::utils::static_vector_typer;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        std::size_t grid_right_cnt,
        std::size_t grid_depth_cnt,
        std::size_t resident_nodes_capacity,
        std::size_t path_edge_capacity
    >
    struct backtracker_stack_container_creator_pack {
        resident_segmenter_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt,
            resident_nodes_capacity
        > create_resident_segmenter_container_creator_pack() {
            return {};
        }

        sliced_subdivider_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt,
            path_edge_capacity
        > create_sliced_subdivider_container_creator_pack() {
            return {};
        }

        using CONTAINER_TYPE = typename static_vector_typer<debug_mode, E, path_edge_capacity>::type;
        CONTAINER_TYPE create_path_container(std::size_t path_edge_capacity_) const  {
            if constexpr (debug_mode) {
                if (path_edge_capacity != path_edge_capacity_) {
                    throw std::runtime_error { "Size mismatch" };
                }
            }
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BACKTRACKER_STACK_CONTAINER_CREATOR_PACK_H
