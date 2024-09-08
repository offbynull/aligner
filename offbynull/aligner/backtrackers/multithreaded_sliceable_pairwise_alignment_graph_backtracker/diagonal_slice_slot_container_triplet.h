#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_DIAGONAL_SLICE_SLOT_CONTAINER_TRIPLET_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_DIAGONAL_SLICE_SLOT_CONTAINER_TRIPLET_H

#include <cstddef>
#include <functional>
#include <array>
#include <optional>
#include <utility>
#include <stdexcept>
#include "offbynull/concepts.h"
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/slot.h"
#include "offbynull/aligner/backtrackers/multithreaded_sliceable_pairwise_alignment_graph_backtracker/diagonal_slice_slot_container.h"

namespace offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker
        ::diagonal_slice_slot_container_triplet {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::concepts::backtrackable_node;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::concepts::backtrackable_edge;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::slot::slot;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::diagonal_slice_slot_container
        ::axis;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::diagonal_slice_slot_container
        ::diagonal_slice_slot_container;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::diagonal_slice_slot_container
        ::diagonal_slice_slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::diagonal_slice_slot_container
        ::diagonal_slice_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::diagonal_slice_slot_container
        ::diagonal_slice_slot_container_stack_container_creator_pack;
    using offbynull::concepts::many_same_as;


    template<bool debug_mode, typename T, typename... Ts>
    requires many_same_as<T, Ts...>
    T* swap_to_new_pointer(T* ptr, Ts&... objects) {
        constexpr std::size_t cnt { sizeof...(objects) };
        std::array<std::reference_wrapper<T>, cnt> refs { std::ref(objects)... };
        for (std::size_t i { 0zu }; i < cnt  / 2zu; ++i) {
            if (ptr == &(refs[i * 2zu].get())) {
                return &(refs[i * 2zu + 1zu].get());
            }
        }
        if constexpr (debug_mode) {
            throw std::runtime_error { "No matching pointer found" };
        }
        std::unreachable();
    }


    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G,
        diagonal_slice_slot_container_container_creator_pack<
            typename G::E,
            typename G::ED
        > DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK = diagonal_slice_slot_container_heap_container_creator_pack<
            debug_mode,
            typename G::E,
            typename G::ED
        >
    >
    struct diagonal_slice_slot_container_triplet {
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        diagonal_slice_slot_container<debug_mode, G, DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> slots1;
        diagonal_slice_slot_container<debug_mode, G, DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> slots2;
        diagonal_slice_slot_container<debug_mode, G, DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> slots3;
        diagonal_slice_slot_container<debug_mode, G, DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK>* n_minus_2_slots;
        diagonal_slice_slot_container<debug_mode, G, DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK>* n_minus_1_slots;
        diagonal_slice_slot_container<debug_mode, G, DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK>* n_minus_0_slots;
        axis grid_axis;
        INDEX grid_axis_position;
        INDEX grid_down_cnt;  // copy of g.grid_down_cnt -- here to avoid keeping a reference ot hte graph

        diagonal_slice_slot_container_triplet(
            const G& g,
            DIAGONAL_SLICE_SLOT_CONTAINER_CONTAINER_CREATOR_PACK container_creator_pack = {}
        )
        : slots1 { g, container_creator_pack }
        , slots2 { g, container_creator_pack }
        , slots3 { g, container_creator_pack }
        , n_minus_2_slots { &slots1 }
        , n_minus_1_slots { &slots2 }
        , n_minus_0_slots { &slots3 }
        , grid_axis { axis::DOWN_FROM_TOP_LEFT }
        , grid_axis_position {}
        , grid_down_cnt { g.grid_down_cnt } {}

        // Custom copy/move/copy assignment/move assigned because this class has raw pointer types as members. The default copy/assignment
        // will do a SHALLOW copy of these pointers, meaning they won't be pointing into the copy'd element_container (they'll instead be
        // pointing into the original element_container).
        diagonal_slice_slot_container_triplet(const diagonal_slice_slot_container_triplet& other)
        : slots1 { other.slots1 }
        , slots2 { other.slots2 }
        , slots3 { other.slots3 }
        , n_minus_2_slots {
            swap_to_new_pointer<debug_mode>(
                other.n_minus_2_slot,
                &other.slots1, &slots1,
                &other.slots2, &slots2,
                &other.slots3, &slots3
            )
        }
        , n_minus_1_slots {
            swap_to_new_pointer<debug_mode>(
                other.n_minus_1_slot,
                &other.slots1, &slots1,
                &other.slots2, &slots2,
                &other.slots3, &slots3
            )
        }
        , n_minus_0_slots {
            swap_to_new_pointer<debug_mode>(
                other.n_minus_0_slot,
                &other.slots1, &slots1,
                &other.slots2, &slots2,
                &other.slots3, &slots3
            )
        }
        , grid_axis { other.grid_axis }
        , grid_axis_position { other.grid_axis_position }
        , grid_down_cnt { other.grid_down_cnt } {}

        diagonal_slice_slot_container_triplet(diagonal_slice_slot_container_triplet&& other) noexcept
        : slots1 { std::move(other.slots1) }
        , slots2 { std::move(other.slots2) }
        , slots3 { std::move(other.slots3) }
        , n_minus_2_slots {
            swap_to_new_pointer<debug_mode>(
                other.n_minus_2_slot,
                &other.slots1, &slots1,
                &other.slots2, &slots2,
                &other.slots3, &slots3
            )
        }
        , n_minus_1_slots {
            swap_to_new_pointer<debug_mode>(
                other.n_minus_1_slot,
                &other.slots1, &slots1,
                &other.slots2, &slots2,
                &other.slots3, &slots3
            )
        }
        , n_minus_0_slots {
            swap_to_new_pointer<debug_mode>(
                other.n_minus_0_slot,
                &other.slots1, &slots1,
                &other.slots2, &slots2,
                &other.slots3, &slots3
            )
        }
        , grid_axis { std::move(other.grid_axis) }
        , grid_axis_position { std::move(other.grid_axis_position) }
        , grid_down_cnt { std::move(other.grid_down_cnt) } {}

        diagonal_slice_slot_container_triplet& operator=(const diagonal_slice_slot_container_triplet& other) {
            if (this != &other) { // guard against self-assignment
                slots1 = other.slots1;
                slots2 = other.slots2;
                slots3 = other.slots3;
                n_minus_2_slots = swap_to_new_pointer<debug_mode>(
                    other.n_minus_2_slot,
                    &other.slots1, &slots1,
                    &other.slots2, &slots2,
                    &other.slots3, &slots3
                );
                n_minus_1_slots = swap_to_new_pointer<debug_mode>(
                    other.n_minus_1_slot,
                    &other.slots1, &slots1,
                    &other.slots2, &slots2,
                    &other.slots3, &slots3
                );
                n_minus_0_slots = swap_to_new_pointer<debug_mode>(
                    other.n_minus_0_slot,
                    &other.slots1, &slots1,
                    &other.slots2, &slots2,
                    &other.slots3, &slots3
                );
                grid_axis = other.grid_axis;
                grid_axis_position = other.grid_axis_position;
                grid_down_cnt = other.grid_down_cnt;
            }
            return *this;
        }

        diagonal_slice_slot_container_triplet& operator=(diagonal_slice_slot_container_triplet&& other) noexcept {
            if (this != &other) { // guard against self-assignment
                slots1 = std::move(other.slots1);
                slots2 = std::move(other.slots2);
                slots3 = std::move(other.slots3);
                n_minus_2_slots = swap_to_new_pointer<debug_mode>(
                    other.n_minus_2_slot,
                    &other.slots1, &slots1,
                    &other.slots2, &slots2,
                    &other.slots3, &slots3
                );
                n_minus_1_slots = swap_to_new_pointer<debug_mode>(
                    other.n_minus_1_slot,
                    &other.slots1, &slots1,
                    &other.slots2, &slots2,
                    &other.slots3, &slots3
                );
                n_minus_0_slots = swap_to_new_pointer<debug_mode>(
                    other.n_minus_0_slot,
                    &other.slots1, &slots1,
                    &other.slots2, &slots2,
                    &other.slots3, &slots3
                );
                grid_axis = std::move(other.grid_axis);
                grid_axis_position = std::move(other.grid_axis_position);
                grid_down_cnt = std::move(other.grid_down_cnt);
            }
            return *this;
        }

        void next() {
            switch (grid_axis) {
                case axis::DOWN_FROM_TOP_LEFT: {
                    if (grid_axis_position == grid_down_cnt - 1zu) {
                        grid_axis = axis::RIGHT_FROM_BOTTOM_LEFT;
                        grid_axis_position = 1;
                    } else {
                        ++grid_axis_position;
                    }
                    break;
                }
                case axis::RIGHT_FROM_BOTTOM_LEFT: {
                    ++grid_axis_position;
                    break;
                }
                [[unlikely]] default: {
                    if constexpr (debug_mode) {
                        throw std::runtime_error { "This should never happen" };
                    }
                    std::unreachable();
                }
            }

            auto* new_n_minus_0_slots { n_minus_2_slots };
            n_minus_2_slots = n_minus_1_slots;
            n_minus_1_slots = n_minus_0_slots;
            n_minus_0_slots = new_n_minus_0_slots;
            n_minus_0_slots->reset(grid_axis, grid_axis_position);
        }

        std::optional<std::reference_wrapper<slot<E, ED>>> find(const N& node) {
            // Initially, all "slots" variables are set to the first diagonal, meaning 0's condition check hits first and so the other 2
            // will never return a found item. Similarly, after the first call to next(), found_n_minus_1 and found_n_minus_2 are both set
            // to the first diagonal, meaning 1's check hits first and so 2 will never return a found item.
            auto found_n_minus_0 { n_minus_0_slots->find(node) };
            if (found_n_minus_0.has_value()) {
                return { found_n_minus_0 };
            }
            auto found_n_minus_1 { n_minus_1_slots->find(node) };
            if (found_n_minus_1.has_value()) {
                return { found_n_minus_1 };
            }
            auto found_n_minus_2 { n_minus_2_slots->find(node) };
            if (found_n_minus_2.has_value()) {
                return { found_n_minus_2 };
            }
            return { std::nullopt };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_DIAGONAL_SLICE_SLOT_CONTAINER_TRIPLET_H
