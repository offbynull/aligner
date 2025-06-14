#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SEGMENTER_RESIDENT_SEGMENTER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SEGMENTER_RESIDENT_SEGMENTER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include <variant>
#include <stdexcept>
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_segmenter/hop.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_segmenter/segment.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/bidi_walker/bidi_walker_stack_container_creator_pack.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_segmenter
        ::resident_segmenter_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_segmenter::hop::hop;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_segmenter::segment::segment;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::bidi_walker
        ::bidi_walker_stack_container_creator_pack::bidi_walker_stack_container_creator_pack;
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::range_of_type;
    using offbynull::utils::static_vector_typer;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        std::size_t grid_right_cnt,
        std::size_t grid_depth_cnt,
        std::size_t resident_nodes_capacity
    >
    struct resident_segmenter_stack_container_creator_pack {
        bidi_walker_stack_container_creator_pack<
            debug_mode,
            N,
            E,
            ED,
            grid_right_cnt,
            grid_depth_cnt,
            resident_nodes_capacity
        > create_bidi_walker_container_creator_pack() const {
            return {};
        }

        using RESIDENT_NODE_CONTAINER_TYPE = typename static_vector_typer<debug_mode, N, resident_nodes_capacity>::type;
        RESIDENT_NODE_CONTAINER_TYPE create_resident_node_container(range_of_type<N> auto&& resident_nodes) const {
            return RESIDENT_NODE_CONTAINER_TYPE(resident_nodes.begin(), resident_nodes.end());
        }

        using RESIDENT_EDGE_CONTAINER_TYPE = typename static_vector_typer<debug_mode, E, resident_nodes_capacity>::type;
        RESIDENT_EDGE_CONTAINER_TYPE create_resident_edge_container(std::size_t resident_nodes_capacity_) const {
            if constexpr (debug_mode) {
                if (resident_nodes_capacity != resident_nodes_capacity_) {
                    throw std::runtime_error { "Inconsistent node count" };
                }
            }
            return {};
        }

        static constexpr std::size_t max_segment_cnt { resident_nodes_capacity * 2zu + 1zu };
        using SEGMENT_CONTAINER_TYPE = typename static_vector_typer<
            debug_mode,
            std::variant<
                hop<E>,
                segment<N>
            >,
            max_segment_cnt
        >::type;
        SEGMENT_CONTAINER_TYPE create_segment_container(std::size_t resident_nodes_cnt_) const {
            if constexpr (debug_mode) {
                if (resident_nodes_cnt_ * 2zu + 1zu > max_segment_cnt) {
                    throw std::runtime_error { "Inconsistent node count" };
                }
            }
            return {};
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SEGMENTER_RESIDENT_SEGMENTER_STACK_CONTAINER_CREATOR_PACK_H
