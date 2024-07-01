#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_H

#include <cstddef>
#include <functional>
#include "offbynull/aligner/graph/pairwise_alignment_graph.h"
#include "offbynull/helpers/container_creators.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container {
    using offbynull::aligner::concepts::weight;
    using offbynull::helpers::container_creators::container_creator;
    using offbynull::helpers::container_creators::vector_container_creator;
    using offbynull::concepts::input_iterator_of_type;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::aligner::graph::pairwise_alignment_graph::readable_pairwise_alignment_graph;

    template<typename N, typename E, widenable_to_size_t COUNT, weight WEIGHT>
    struct slot {
        N node;
        COUNT unwalked_parent_cnt;
        E backtracking_edge;
        WEIGHT backtracking_weight;

        slot(N node_, COUNT unwalked_parent_cnt_)
        : node{node_}
        , unwalked_parent_cnt{unwalked_parent_cnt_}
        , backtracking_edge{}
        , backtracking_weight{} {}


        slot()
        : node{}
        , unwalked_parent_cnt{}
        , backtracking_edge{}
        , backtracking_weight{} {}
    };

    template<
        typename N,
        typename E,
        widenable_to_size_t INDEX,
        widenable_to_size_t COUNT,
        weight WEIGHT,
        container_creator CONTAINER_CREATOR=vector_container_creator<slot<N, E, COUNT, WEIGHT>>,
        bool error_check=true
    >
    class slot_container {
    private:
        INDEX grid_down_cnt;
        INDEX grid_right_cnt;
        std::size_t max_grid_node_depth;
        std::function<std::tuple<INDEX, INDEX, std::size_t>(const N&)> node_to_grid_offsets;
        decltype(std::declval<CONTAINER_CREATOR>().create_empty(std::nullopt)) slots;

    public:
        slot_container(
            INDEX grid_down_cnt_,
            INDEX grid_right_cnt_,
            std::size_t max_grid_node_depth_,
            std::function<std::tuple<INDEX, INDEX, std::size_t>(const N&)> node_to_grid_offsets_,
            /*input_iterator_of_type<slot<N, E, COUNT, WEIGHT>>*/ auto begin,
            /*std::sentinel_for<decltype(begin)>*/ auto end,
            CONTAINER_CREATOR container_creator = {}
        )
        : grid_down_cnt { grid_down_cnt_ }
        , grid_right_cnt { grid_right_cnt_ }
        , max_grid_node_depth { max_grid_node_depth_ }
        , node_to_grid_offsets { node_to_grid_offsets_ }
        , slots { container_creator.create_objects(max_grid_node_depth * (grid_down_cnt * grid_right_cnt)) } {
            auto it { begin };
            while (it != end) {
                const auto& slot { *it };
                const auto& [down_offset, right_offset, depth] { node_to_grid_offsets(slot.node) };
                std::size_t idx { (max_grid_node_depth * ((down_offset * grid_right_cnt) + right_offset)) + depth };
                slots[idx] = slot;
                ++it;
            }
        }

        std::size_t find_idx(const N& node){
            const auto& [down_offset, right_offset, depth] { node_to_grid_offsets(node) };
            return (max_grid_node_depth * ((down_offset * grid_right_cnt) + right_offset)) + depth;
        }

        slot<N, E, COUNT, WEIGHT>& find_ref(const N& node) {
            std::size_t idx { find_idx(node) };
            slot<N, E, COUNT, WEIGHT>& slot { slots[idx] };
            return slot;
        }

        slot<N, E, COUNT, WEIGHT>& at_idx(const std::size_t idx) {
            return slots[idx];
        }

        std::pair<std::size_t, slot<N, E, COUNT, WEIGHT>&> find(const N& node) {
            std::size_t idx { find_idx(node) };
            slot<N, E, COUNT, WEIGHT>& slot { slots[idx] };
            return { idx, slot };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_H
