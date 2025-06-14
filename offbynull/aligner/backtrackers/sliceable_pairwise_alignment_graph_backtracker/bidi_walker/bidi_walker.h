#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BIDI_WALKER_BIDI_WALKER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BIDI_WALKER_BIDI_WALKER_H

#include <utility>
#include <ranges>
#include <cmath>
#include <type_traits>
#include <stdexcept>
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/bidi_walker/bidi_walker_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/bidi_walker/bidi_walker_heap_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/forward_walker/forward_walker.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/slot.h"
#include "offbynull/aligner/graphs/reversed_sliceable_pairwise_alignment_graph.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::bidi_walker::bidi_walker {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::bidi_walker
        ::bidi_walker_container_creator_pack::bidi_walker_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::bidi_walker
        ::bidi_walker_heap_container_creator_pack::bidi_walker_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::forward_walker::forward_walker::forward_walker;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container::slot::slot;
    using offbynull::aligner::graphs::reversed_sliceable_pairwise_alignment_graph::reversed_sliceable_pairwise_alignment_graph;

    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G,
        bidi_walker_container_creator_pack<
            typename G::N,
            typename G::E,
            typename G::ED
        > CONTAINER_CREATOR_PACK = bidi_walker_heap_container_creator_pack<
            debug_mode,
            typename G::N,
            typename G::E,
            typename G::ED,
            true
        >
    >
    requires backtrackable_node<typename G::N> &&
        backtrackable_edge<typename G::E>
    class bidi_walker {
    public:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

    private:
        using FORWARD_WALKER_CONTAINER_CREATOR_PACK =
            decltype(std::declval<CONTAINER_CREATOR_PACK>().create_forward_walker_container_creator_pack());
        using BACKWARD_WALKER_CONTAINER_CREATOR_PACK =
            decltype(std::declval<CONTAINER_CREATOR_PACK>().create_backward_walker_container_creator_pack());

        const G& g;
        const INDEX target_row;
        const reversed_sliceable_pairwise_alignment_graph<debug_mode, G> reversed_g;
        forward_walker<debug_mode, G, FORWARD_WALKER_CONTAINER_CREATOR_PACK> forward_walker_;
        forward_walker<debug_mode, std::remove_const_t<decltype(reversed_g)>, BACKWARD_WALKER_CONTAINER_CREATOR_PACK> backward_walker;

    public:
        static bidi_walker create_and_initialize(
            const G& g_,
            const INDEX target_row,
            CONTAINER_CREATOR_PACK container_creator_pack = {}
        ) {
            if constexpr (debug_mode) {
                if (target_row >= g_.grid_down_cnt) {
                    throw std::runtime_error { "Slice too far down" };
                }
            }
            bidi_walker ret {
                g_,
                target_row,
                container_creator_pack
            };
            return ret;
        }

        struct find_result {
            const slot<E, ED>& forward_slot;
            const slot<E, ED>& backward_slot;
        };

        find_result find(const N& node) {
            const auto& forward_slot { forward_walker_.find(node) };
            const auto& backward_slot { backward_walker.find(node) };
            return { forward_slot, backward_slot };
        }

        struct list_entry {
            N node;
            find_result slots;
        };

        auto list() {
            return g.row_nodes(target_row)
                | std::views::transform([&](const N& n) {
                    return list_entry { n, find(n) };
                });
        }

        // Equivalent to find_result, but slots are NOT REFERENCES into the bidi_walker. That way, if the bidi_walker goes out of scope,
        // it won't be pointing to released memory.
        struct find_result_copy {
            const slot<E, ED> forward_slot;
            const slot<E, ED> backward_slot;
        };

        static find_result_copy converge(
            const G& g,
            const N& node
        ) {
            const auto& [down, right, depth] { g.node_to_grid_offset(node) };
            bidi_walker bidi_walker_ { bidi_walker::create_and_initialize(g, down) };
            find_result found { bidi_walker_.find(node) };
            return find_result_copy { found.forward_slot, found.backward_slot };
        }

        static ED converge_weight(
            const G& g,
            const N& node
        ) {
            find_result_copy slots { converge(g, node) };
            return slots.forward_slot.backtracking_weight + slots.backward_slot.backtracking_weight;
        }

        static bool is_node_on_max_path(
            const G& g_,
            const typename G::N& node,
            const typename G::ED max_path_weight,
            const typename G::ED max_path_weight_comparison_tolerance
        ) {
            const auto& [down, right, depth] { g_.node_to_grid_offset(node) };

            bidi_walker bidi_walker_ { bidi_walker::create_and_initialize(g_, down) };
            for (const auto& entry : bidi_walker_.list()) {
                ED node_converged_weight { entry.slots.forward_slot.backtracking_weight + entry.slots.backward_slot.backtracking_weight };
                if (std::abs(node_converged_weight -  max_path_weight) <= max_path_weight_comparison_tolerance) {
                    return true;
                }
            }
            return false;
        }

    private:
        bidi_walker(
            const G& g_,
            const INDEX target_row_,
            CONTAINER_CREATOR_PACK container_creator_pack_ = {}
        )
        : g { g_ }
        , target_row { target_row_ }
        , reversed_g { g }
        , forward_walker_ {
            decltype(forward_walker_)::create_and_initialize(
                g,
                target_row,
                container_creator_pack_.create_forward_walker_container_creator_pack()
            )
        }
        , backward_walker {
            decltype(backward_walker)::create_and_initialize(
                reversed_g,
                g.grid_down_cnt - 1u - target_row,
                container_creator_pack_.create_backward_walker_container_creator_pack()
            )
        } {}
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_BIDI_WALKER_BIDI_WALKER_H
