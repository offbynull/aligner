#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_H

#include <cstddef>
#include <iterator>
#include <utility>
#include <limits>
#include <stdexcept>
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/slot_container/slot.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/slot_container/slot_container_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/slot_container/slot_container_heap_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/graph/pairwise_alignment_graph.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot_container {
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::concepts::unqualified_object_type;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::aligner::graph::pairwise_alignment_graph::readable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot::slot;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot_container_container_creator_pack
        ::slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::slot_container::slot_container_heap_container_creator_pack
        ::slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;

    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G,
        widenable_to_size_t PARENT_COUNT,
        slot_container_container_creator_pack<
            typename G::N,
            typename G::E,
            typename G::ED,
            PARENT_COUNT
        > CONTAINER_CREATOR_PACK = slot_container_heap_container_creator_pack<
            debug_mode,
            typename G::N,
            typename G::E,
            typename G::ED,
            PARENT_COUNT
        >
    >
    class slot_container {
    private:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        using SLOT_CONTAINER = decltype(std::declval<CONTAINER_CREATOR_PACK>().create_slot_container(0zu, 0zu, 0zu));

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
        , slots {
            container_creator_pack.create_slot_container(
                g.grid_down_cnt,
                g.grid_right_cnt,
                g.grid_depth_cnt
            )
        } {
            if constexpr (debug_mode) {
                if (std::numeric_limits<PARENT_COUNT>::max() < g.node_incoming_edge_capacity) {
                    throw std::runtime_error { "PARENT_COUNT not wide enough to support node_incoming_edge_capacity" };
                }
            }
            auto it { begin };
            while (it != end) {
                const auto& slot { *it };
                const auto& [down_offset, right_offset, depth] { g.node_to_grid_offset(slot.node) };
                std::size_t idx { (g.grid_depth_cnt * ((down_offset * g.grid_right_cnt) + right_offset)) + depth };
                slots[idx] = slot;
                ++it;
            }
        }

        std::size_t find_idx(const N& node) const {
            const auto& [down_offset, right_offset, depth] { g.node_to_grid_offset(node) };
            return (g.grid_depth_cnt * ((down_offset * g.grid_right_cnt) + right_offset)) + depth;
        }

        slot<N, E, ED, PARENT_COUNT>& find_ref(const N& node) {
            std::size_t idx { find_idx(node) };
            slot<N, E, ED, PARENT_COUNT>& slot { slots[idx] };
            return slot;
        }

        const slot<N, E, ED, PARENT_COUNT>& find_ref(const N& node) const {
            std::size_t idx { find_idx(node) };
            const slot<N, E, ED, PARENT_COUNT>& slot { slots[idx] };
            return slot;
        }

        slot<N, E, ED, PARENT_COUNT>& at_idx(const std::size_t idx) {
            return slots[idx];
        }

        const slot<N, E, ED, PARENT_COUNT>& at_idx(const std::size_t idx) const {
            return slots[idx];
        }

        std::pair<std::size_t, slot<N, E, ED, PARENT_COUNT>&> find(const N& node) {
            std::size_t idx { find_idx(node) };
            slot<N, E, ED, PARENT_COUNT>& slot { slots[idx] };
            return { idx, slot };
        }

        std::pair<std::size_t, const slot<N, E, ED, PARENT_COUNT>&> find(const N& node) const {
            std::size_t idx { find_idx(node) };
            const slot<N, E, ED, PARENT_COUNT>& slot { slots[idx] };
            return { idx, slot };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_CONTAINER_SLOT_CONTAINER_H
