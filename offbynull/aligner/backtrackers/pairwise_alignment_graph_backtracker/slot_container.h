#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_H

#include <cstddef>
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/utils.h"
#include "offbynull/aligner/graph/pairwise_alignment_graph.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container {
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::aligner::graph::pairwise_alignment_graph::readable_pairwise_alignment_graph;

    template<typename N, typename E, weight ED>
    struct slot {
        N node;
        std::size_t unwalked_parent_cnt;
        E backtracking_edge;
        ED backtracking_weight;

        slot(N node_, std::size_t unwalked_parent_cnt_)
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
        typename T,
        typename G
    >
    concept slot_container_container_creator_pack =
    readable_pairwise_alignment_graph<G>
    && requires(T t, const G& g) {
        { t.create_slot_container(g) } -> random_access_range_of_type<slot<typename G::N, typename G::E, typename G::ED>>;
    };

    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G
    >
    struct slot_container_heap_container_creator_pack {
        using N = typename G::N;
        using E = typename G::E;
        using ED = typename G::ED;

        std::vector<slot<N, E, ED>> create_slot_container(const G& g) {
            std::size_t cnt { (g.grid_down_cnt * g.grid_right_cnt) * G::limits(g.grid_down_cnt, g.grid_right_cnt).max_grid_node_depth };
            return std::vector<slot<N, E, ED>>(cnt);
        }
    };

    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G,
        std::size_t grid_down_cnt,
        std::size_t grid_right_cnt
    >
    struct slot_container_stack_container_creator_pack {
        using N = typename G::N;
        using E = typename G::E;
        using ED = typename G::ED;

        static constexpr std::size_t ELEM_COUNT { grid_down_cnt * grid_right_cnt * G::limits(grid_down_cnt, grid_right_cnt).max_grid_node_depth };

        std::array<slot<N, E, ED>, ELEM_COUNT> create_slot_container(const G& g) {
            if constexpr (debug_mode) {
                std::size_t cnt { (g.grid_down_cnt * g.grid_right_cnt) * G::limits(g.grid_down_cnt, g.grid_right_cnt).max_grid_node_depth };
                if (cnt != ELEM_COUNT) {
                    throw std::runtime_error("Bad element count");
                }
            }
            return std::array<slot<N, E, ED>, ELEM_COUNT> {};
        }
    };





    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G,
        slot_container_container_creator_pack<G> CONTAINER_CREATOR_PACK=slot_container_heap_container_creator_pack<debug_mode, G>
    >
    class slot_container {
    private:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        using SLOT_CONTAINER=decltype(std::declval<CONTAINER_CREATOR_PACK>().create_slot_container(std::declval<G>()));

        const G& g;
        SLOT_CONTAINER slots;

    public:
        slot_container(
            const G& g_,
            /*input_iterator_of_type<slot<N, E, WEIGHT>>*/ auto begin,
            /*std::sentinel_for<decltype(begin)>*/ auto end,
            CONTAINER_CREATOR_PACK container_creator_pack = {}
        )
        : g { g_ }
        , slots { container_creator_pack.create_slot_container(g) } {
            auto it { begin };
            while (it != end) {
                const auto& slot { *it };
                const auto& [down_offset, right_offset, depth] { g.node_to_grid_offsets(slot.node) };
                const auto& max_grid_node_depth { G::limits(g.grid_down_cnt, g.grid_right_cnt).max_grid_node_depth };
                std::size_t idx { (max_grid_node_depth * ((down_offset * g.grid_right_cnt) + right_offset)) + depth };
                slots[idx] = slot;
                ++it;
            }
        }

        std::size_t find_idx(const N& node){
            const auto& [down_offset, right_offset, depth] { g.node_to_grid_offsets(node) };
            const auto& max_grid_node_depth { G::limits(g.grid_down_cnt, g.grid_right_cnt).max_grid_node_depth };
            return (max_grid_node_depth * ((down_offset * g.grid_right_cnt) + right_offset)) + depth;
        }

        slot<N, E, ED>& find_ref(const N& node) {
            std::size_t idx { find_idx(node) };
            slot<N, E, ED>& slot { slots[idx] };
            return slot;
        }

        slot<N, E, ED>& at_idx(const std::size_t idx) {
            return slots[idx];
        }

        std::pair<std::size_t, slot<N, E, ED>&> find(const N& node) {
            std::size_t idx { find_idx(node) };
            slot<N, E, ED>& slot { slots[idx] };
            return { idx, slot };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_H
