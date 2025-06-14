#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_H

#include "offbynull/utils.h"
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/backward_walker_range.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/path_container_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/path_container_heap_container_creator_pack.h"
#include <cstddef>
#include <ranges>
#include <utility>
#include <stdexcept>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::path_container {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::backward_walker_range
        ::backward_walker_range;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::path_container_container_creator_pack::path_container_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container
        ::path_container_heap_container_creator_pack::path_container_heap_container_creator_pack;

    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G,
        path_container_container_creator_pack<
            typename G::E
        > CONTAINER_CREATOR_PACK = path_container_heap_container_creator_pack<
            debug_mode,
            typename G::E
        >
    >
    class path_container {
    private:
        using E = typename G::E;

        using ELEMENT_CONTAINER = decltype(std::declval<CONTAINER_CREATOR_PACK>().create_element_container(0zu));

        ELEMENT_CONTAINER element_container;
        element<E>* head;
        element<E>* tail;
        std::size_t next_idx;

    public:
        path_container(
            const G& g,
            CONTAINER_CREATOR_PACK container_creator_pack = {}
        )
        : element_container {
            container_creator_pack.create_element_container(g.path_edge_capacity)
        }
        , head { nullptr }
        , tail { nullptr }
        , next_idx { 0zu } {}

        // Custom copy/move/copy assignment/move assigned because this class has raw pointer types as members. The default copy/assignment
        // will do a SHALLOW copy of these pointers, meaning they won't be pointing into the copy'd element_container (they'll instead be
        // pointing into the original element_container).
        path_container(const path_container& other)
        : element_container { other.element_container }
        , head { other.head == nullptr ? nullptr : &element_container[other.head - &(other.element_container[0zu])] }
        , tail { other.tail == nullptr ? nullptr : &element_container[other.tail - &(other.element_container[0zu])] }
        , next_idx { other.next_idx } {
            for (std::size_t i { 0zu }; i < other.next_idx; ++i) {
                element<E>& other_element { element_container[i] };
                element<E>& this_element { element_container[i] };
                this_element.backtracking_edge = other_element.backtracking_edge;
                this_element.prev = &element_container[other_element.prev - &(other.element_container[0zu])];
                this_element.next = &element_container[other_element.next - &(other.element_container[0zu])];
            }
        }

        path_container(path_container&& other) noexcept
        : element_container { other.element_container }
        , head { other.head == nullptr ? nullptr : &element_container[other.head - &(other.element_container[0zu])] }
        , tail { other.tail == nullptr ? nullptr : &element_container[other.tail - &(other.element_container[0zu])] }
        , next_idx { other.next_idx } {
            for (std::size_t i { 0zu }; i < other.next_idx; ++i) {
                element<E>& other_element { element_container[i] };
                element<E>& this_element { element_container[i] };
                this_element.backtracking_edge = other_element.backtracking_edge;
                this_element.prev = &element_container[other_element.prev - &(other.element_container[0zu])];
                this_element.next = &element_container[other_element.next - &(other.element_container[0zu])];
            }
            other.head = nullptr;
            other.tail = nullptr;
            other.next_idx = 0zu;
        }

        path_container& operator=(const path_container& other) {
            if (this != &other) { // guard against self-assignment
                element_container = other.element_container;
                head = other.head == nullptr ? nullptr : &element_container[other.head - &(other.element_container[0zu])];
                tail = other.tail == nullptr ? nullptr : &element_container[other.tail - &(other.element_container[0zu])];
                next_idx = other.next_idx;
                for (std::size_t i { 0zu }; i < other.next_idx; ++i) {
                    element<E>& other_element { element_container[i] };
                    element<E>& this_element { element_container[i] };
                    this_element.backtracking_edge = other_element.backtracking_edge;
                    this_element.prev = &element_container[other_element.prev - &(other.element_container[0zu])];
                    this_element.next = &element_container[other_element.next - &(other.element_container[0zu])];
                }
            }
            return *this;
        }

        path_container& operator=(path_container&& other) noexcept {
            if (this != &other) { // guard against self-assignment
                element_container = other.element_container;
                head = other.head == nullptr ? nullptr : &element_container[other.head - &(other.element_container[0zu])];
                tail = other.tail == nullptr ? nullptr : &element_container[other.tail - &(other.element_container[0zu])];
                next_idx = other.next_idx;
                for (std::size_t i { 0zu }; i < other.next_idx; ++i) {
                    element<E>& other_element { element_container[i] };
                    element<E>& this_element { element_container[i] };
                    this_element.backtracking_edge = other_element.backtracking_edge;
                    this_element.prev = &element_container[other_element.prev - &(other.element_container[0zu])];
                    this_element.next = &element_container[other_element.next - &(other.element_container[0zu])];
                    other.head = nullptr;
                    other.tail = nullptr;
                    other.next_idx = 0zu;
                }
            }
            return *this;
        }

        element<E>* initialize(const E& backtracking_edge) {
            if constexpr (debug_mode) {
                if (next_idx != 0zu) {
                    throw std::runtime_error { "Already initialized" };
                }
                if (next_idx >= element_container.size()) {
                    // If this happens, G::path_edge_capacity is probably giving back a number that's too low
                    throw std::runtime_error { "Container too small" };
                }
            }
            element<E>* entry { &element_container[0zu] };
            entry->backtracking_edge = backtracking_edge;
            ++next_idx;
            head = entry;
            tail = entry;
            return entry;
        }

        element<E>* push_prefix(element<E>* entry, const E& backtracking_edge) {
            if constexpr (debug_mode) {
                if (next_idx == 0zu) {
                    throw std::runtime_error { "Not initialized" };
                }
                if (next_idx >= element_container.size()) {
                    // If this happens, G::path_edge_capacity is probably giving back a number that's too low
                    throw std::runtime_error { "Container too small" };
                }
            }
            element<E>* prefix_entry { &element_container[next_idx] };
            prefix_entry->backtracking_edge = backtracking_edge;
            ++next_idx;
            element<E>* orig_prev { entry->prev };
            if (orig_prev != nullptr) {
                orig_prev->next = prefix_entry;
            }
            prefix_entry->prev = orig_prev;
            entry->prev = prefix_entry;
            prefix_entry->next = entry;
            if (entry == head) {
                head = prefix_entry;
            }
            return prefix_entry;
        }

        element<E>* push_suffix(element<E>* entry, const E& backtracking_edge) {
            if constexpr (debug_mode) {
                if (next_idx == 0zu) {
                    throw std::runtime_error { "Not initialized" };
                }
                if (next_idx >= element_container.size()) {
                    // If this happens, G::path_edge_capacity is probably giving back a number that's too low
                    throw std::runtime_error { "Container too small" };
                }
            }
            element<E>* suffix_entry { &element_container[next_idx] };
            suffix_entry->backtracking_edge = backtracking_edge;
            ++next_idx;
            element<E>* orig_next { entry->next };
            if (orig_next != nullptr) {
                orig_next->prev = suffix_entry;
            }
            suffix_entry->next = orig_next;
            entry->next = suffix_entry;
            suffix_entry->prev = entry;
            if (entry == tail) {
                tail = suffix_entry;
            }
            return suffix_entry;
        }

        std::ranges::forward_range auto walk_path_backward() {
            return backward_walker_range<G> { head, tail };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_PATH_CONTAINER_H
