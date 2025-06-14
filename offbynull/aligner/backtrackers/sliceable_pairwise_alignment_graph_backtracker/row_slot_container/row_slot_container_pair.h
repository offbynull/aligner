#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_ROW_SLOT_CONTAINER_ROW_SLOT_CONTAINER_PAIR_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_ROW_SLOT_CONTAINER_ROW_SLOT_CONTAINER_PAIR_H

#include <functional>
#include <optional>
#include <utility>
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/slot.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/row_slot_container/row_slot_container.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container::row_slot_container_pair {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container::slot::slot;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container::row_slot_container;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_container_creator_pack::row_slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::row_slot_container
        ::row_slot_container_heap_container_creator_pack::row_slot_container_heap_container_creator_pack;

    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G,
        row_slot_container_container_creator_pack<
            typename G::E,
            typename G::ED
        > ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK = row_slot_container_heap_container_creator_pack<
            debug_mode,
            typename G::E,
            typename G::ED
        >
    >
    struct row_slot_container_pair {
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;

        row_slot_container<debug_mode, G, ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> slots1;
        row_slot_container<debug_mode, G, ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK> slots2;
        row_slot_container<debug_mode, G, ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK>* previous_slots;  // row above current row
        row_slot_container<debug_mode, G, ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK>* current_slots;  // current row
        INDEX grid_down_offset;

        row_slot_container_pair(
            const G& g,
            ROW_SLOT_CONTAINER_CONTAINER_CREATOR_PACK container_creator_pack = {}
        )
        : slots1 { g, container_creator_pack }
        , slots2 { g, container_creator_pack }
        , previous_slots { &slots1 }
        , current_slots { &slots2 }
        , grid_down_offset { 0u } {}

        // Custom copy/move/copy assignment/move assigned because this class has raw pointer types as members. The default copy/assignment
        // will do a SHALLOW copy of these pointers, meaning they won't be pointing into the copy'd element_container (they'll instead be
        // pointing into the original element_container).
        row_slot_container_pair(const row_slot_container_pair& other)
        : slots1 { other.slots1 }
        , slots2 { other.slots2 }
        , previous_slots { other.previous_slots == &other.slots1 ? &slots1 : &slots2 }
        , current_slots { other.current_slots == &other.slots1 ? &slots1 : &slots2 }
        , grid_down_offset { other.grid_down_offset } {}

        row_slot_container_pair(row_slot_container_pair&& other) noexcept
        : slots1 { std::move(other.slots1) }
        , slots2 { std::move(other.slots2) }
        , previous_slots { other.previous_slots == &other.slots1 ? &slots1 : &slots2 }
        , current_slots { other.current_slots == &other.slots1 ? &slots1 : &slots2 }
        , grid_down_offset { std::move(other.grid_down_offset) } {}

        row_slot_container_pair& operator=(const row_slot_container_pair& other) {
            if (this != &other) { // guard against self-assignment
                slots1 = other.slots1;
                slots2 = other.slots2;
                previous_slots = other.previous_slots == &other.slots1 ? &slots1 : &slots2;
                current_slots = other.current_slots == &other.slots1 ? &slots1 : &slots2;
                grid_down_offset = other.grid_down_offset;
            }
            return *this;
        }

        row_slot_container_pair& operator=(row_slot_container_pair&& other) noexcept {
            if (this != &other) { // guard against self-assignment
                slots1 = std::move(other.slots1);
                slots2 = std::move(other.slots2);
                previous_slots = other.previous_slots == &other.slots1 ? &slots1 : &slots2;
                current_slots = other.current_slots == &other.slots1 ? &slots1 : &slots2;
                grid_down_offset = std::move(other.grid_down_offset);
            }
            return *this;
        }

        void move_down() {
            ++grid_down_offset;

            auto* old_previous_slots { previous_slots };
            previous_slots = current_slots;
            current_slots = old_previous_slots;

            current_slots->reset(grid_down_offset);
        }

        std::optional<std::reference_wrapper<slot<E, ED>>> find(const N& node) {
            auto found_lower { current_slots->find(node) };
            if (found_lower.has_value()) {
                return { found_lower };
            }
            auto found_upper { previous_slots->find(node) };
            if (found_upper.has_value()) {
                return { found_upper };
            }
            return { std::nullopt };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_ROW_SLOT_CONTAINER_ROW_SLOT_CONTAINER_PAIR_H
