#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_DIAGONAL_FORWARD_WALKER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_DIAGONAL_FORWARD_WALKER_H

#include <cstddef>
#include <functional>
#include <ranges>
#include <optional>
#include <utility>
#include <stdexcept>
#include <any>
#include <future>
#include <algorithm>
#include <vector>
#include <shared_mutex>
#include "offbynull/aligner/concepts.h"
#include "offbynull/helpers/forkable_thread_pool.h"
#include "offbynull/aligner/graph/multithreaded_sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/slot.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/diagonal_slice_slot_container.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/\
diagonal_slice_slot_container_triplet.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/resident_slot_container.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::forward_walker {
    using offbynull::helpers::forkable_thread_pool::forkable_thread_pool;
    using offbynull::aligner::graph::multithreaded_sliceable_pairwise_alignment_graph
        ::readable_multithreaded_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::graph::multithreaded_sliceable_pairwise_alignment_graph::axis;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::concepts::backtrackable_node;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::concepts::backtrackable_edge;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::slot::slot;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker
        ::diagonal_slice_slot_container_triplet::diagonal_slice_slot_container_triplet;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_with_node;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_stack_container_creator_pack;
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::range_of_type;
    using offbynull::concepts::widenable_to_size_t;
    using offbynull::concepts::unqualified_object_type;


    template<
        bool debug_mode,
        widenable_to_size_t INDEX,
        weight WEIGHT
    >
    struct diagonal_segment {
        std::pair<INDEX, INDEX> grid_start;
        std::pair<INDEX, INDEX> grid_stop; // inclusive
        std::shared_mutex grid_stop_rw_mutex;

        diagonal_segment()
        : grid_start { grid_start }
        , grid_stop { grid_stop }
        , grid_stop_rw_mutex {} {}
    };


    template<
        bool debug_mode,
        readable_multithreaded_sliceable_pairwise_alignment_graph G
    >
    class forward_walker_task {
    private:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        const G& g;
        resident_slot_container<debug_mode, G> resident_slots;
        diagonal_slice_slot_container_triplet<debug_mode, G> slice_slots;


    public:
        forward_walker_task(
            const G& g_,
            CONTAINER_CREATOR_PACK container_creator_pack_ = {}
        )
        : g { g_ }
        , resident_slots { g, container_creator_pack_.create_resident_slot_container_container_creator_pack() }
        , slice_slots { g, container_creator_pack_.create_slice_slot_container_container_creator_pack() } {}

        /*
                          1
                          2 2
         1 2 3 4 5        3 3 3
         2 3 4 5 6        4 4 4
         3 4 5 6 7        5 5 5
                          6 6
                          7
         */

        void operator()(forkable_thread_pool<debug_mode, std::any>& pool) const {
            const INDEX slice_len { std::min(g.grid_down_cnt, g.grid_right_cnt) };
            const INDEX slice_len_step_per_core { slice_len / pool.concurrency };
            const std::size_t max_iterations { g.grid_down_cnt + g.grid_right_cnt - 1zu };
            for (std::size_t i {}; i < max_iterations; ++i) {
                if (slice_slots.grid_axis == axis::RIGHT_FROM_BOTTOM_LEFT && slice_slots.grid_axis_position != g.grid_right_cnt - 1u) {
                    break;
                }
                std::vector<std::future<std::any>> subtask_futures(pool.concurrency);
                std::size_t subtask_slice_begin_offset {};
                std::size_t subtask_slice_end_offset {};
                for (std::size_t i { 0zu }; i < pool.concurrency - 1zu; ++i) {
                    subtask_slice_begin_offset = i * slice_len_step_per_core;
                    subtask_slice_end_offset = (i + 1zu) * slice_len_step_per_core;
                    subtask_futures.emplace_back(*pool.queue(SUBTASK {}));
                }
                // Last should consume remainder, otherwise rounding error may cause truncation
                subtask_slice_begin_offset = subtask_slice_end_offset;
                subtask_slice_end_offset = slice_len - subtask_slice_begin_offset;
                subtask_futures.emplace_back(*pool.queue(SUBTASK {}));
                // Submit to pool
                for (std::future<std::any>& future : subtask_futures) {
                    pool.join(future);
                }
                // NOW TO ACCESS, THEY NEED TO USE find() FUNCTION BELOW
                // NOW TO ACCESS, THEY NEED TO USE find() FUNCTION BELOW
                // NOW TO ACCESS, THEY NEED TO USE find() FUNCTION BELOW
            }
        }

        slot<E, ED>& find(const N& node) {
            auto found_resident { resident_slots.find(node) };
            if (found_resident.has_value()) {
                return (*found_resident).get().slot_;
            }
            auto found_slice { slice_slots.find(node) };
            if (found_slice.has_value()) {
                return *found_slice;
            }
            if constexpr (debug_mode) {
                throw std::runtime_error { "Node not found" };
            }
            std::unreachable();
        }
    };

    template<
        bool debug_mode,
        readable_multithreaded_sliceable_pairwise_alignment_graph G,
        forward_walker_container_creator_pack<
            typename G::N,
            typename G::E,
            typename G::ED
        > CONTAINER_CREATOR_PACK
    >
    class forward_walker_segment_task {
    private:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        forward_walker_task<debug_mode, G, CONTAINER_CREATOR_PACK>& parent;
        std::size_t segment_idx;
        std::size_t slice_begin_offset;
        std::size_t slice_end_offset;

        void operator()(forkable_thread_pool<debug_mode, std::any>& pool) const {
            N slice_start { parent.g.diagonal_nodes(parent.grid_axis, parent.grid_axis_position).begin() };
            N slice_begin { slice_start + slice_begin_offset };
            N slice_end { slice_start + slice_end_offset };
            N slice { parent.g.diagonal_nodes(parent.grid_axis, parent.grid_axis_position, slice_start, slice_end) };
            for (std::size_t i { slice_begin_offset }; i < slice_end_offset; ++i) {
                parent.g.get_inputs();
            }
        }

        void step_forward() {
            if (slice_it != slice.end()) {
                slice_entry_.node = *slice_it;
                slice_entry_.slot_ptr = &find(slice_entry_.node);
            } else {
                if (slice_slots.grid_down_offset == g.grid_down_cnt - 1u) {
                    return;
                }
                slice_slots.move_down();
                slice = g.row_nodes(slice_slots.grid_down_offset);
                slice_it = slice.begin();
                slice_entry_.node = *slice_it;
                slice_entry_.slot_ptr = &find(slice_entry_.node);
            }
            // Compute only if node is not a resident. A resident node's backtracking weight + backtracking edge should
            // be computed as its inputs are walked over one-by-one by this function (see block below this one).
            if (!resident_slots.find(slice_entry_.node).has_value()) {
                auto incoming_accumulated {
                    std::views::common(
                        g.get_inputs(slice_entry_.node)
                        | std::views::transform(
                            [&](const auto& edge) -> std::pair<E, ED> {
                                const N& n_from { g.get_edge_from(edge) };
                                const ED& edge_weight { g.get_edge_data(edge) };
                                slot<E, ED>& n_from_slot { find(n_from) };
                                return { edge, n_from_slot.backtracking_weight + edge_weight };
                            }
                        )
                    )
                };
                auto found {
                    std::ranges::max_element(
                        incoming_accumulated.begin(),
                        incoming_accumulated.end(),
                        [](const std::pair<E, ED>& a, const std::pair<E, ED>& b) {
                            return a.second < b.second;
                        }
                    )
                };
                if (found != incoming_accumulated.end()) {  // if no incoming nodes found, it's a root node
                    slice_entry_.slot_ptr->backtracking_edge = { (*found).first };
                    slice_entry_.slot_ptr->backtracking_weight = (*found).second;
                }
            }

            // Update resident node weights
            for (const E& edge : g.outputs_to_residents(slice_entry_.node)) {
                const N& resident_node { g.get_edge_to(edge) };
                std::optional<std::reference_wrapper<resident_slot<E, ED>>> resident_slot_maybe {
                    resident_slots.find(resident_node)
                };
                if constexpr (debug_mode) {
                    if (!resident_slot_maybe.has_value()) {
                        throw std::runtime_error { "This should never happen" };
                    }
                }
                resident_slot<E, ED>& resident_slot_ { (*resident_slot_maybe).get() };
                const ED& edge_weight { g.get_edge_data(edge) };
                if (!resident_slot_.initialized) {
                    resident_slot_.slot_.backtracking_edge = { edge };
                    resident_slot_.slot_.backtracking_weight = edge_weight;
                    resident_slot_.initialized = true;
                } else {
                    const ED& new_weight { slice_entry_.slot_ptr->backtracking_weight + edge_weight };
                    if (new_weight > resident_slot_.slot_.backtracking_weight) {
                        resident_slot_.slot_.backtracking_edge = { edge };
                        resident_slot_.slot_.backtracking_weight = new_weight;
                    }
                }
            }

            // Move to next node
            ++slice_it;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_DIAGONAL_FORWARD_WALKER_H
