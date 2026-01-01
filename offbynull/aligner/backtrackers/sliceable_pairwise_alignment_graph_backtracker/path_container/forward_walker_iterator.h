#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_FORWARD_WALKER_ITERATOR_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_FORWARD_WALKER_ITERATOR_H

#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include <cstdint>
#include <iterator>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::forward_walker_iterator {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::sliceable_pairwise_alignment_graph;

    /**
     * Iterator that walks the edges of an
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::path_container::path_container
     * in reverse.
     *
     * @tparam E Graph edge identifier type.
     */
    template<backtrackable_edge E>
    class forward_walker_iterator {
    private:
        element<E>* head;
        element<E>* tail;
        element<E>* current;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = E;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::bidirectional_iterator_tag;

        forward_walker_iterator()
        : head {}
        , tail {}
        , current {} {}

        forward_walker_iterator(
            element<E>* head_,
            element<E>* tail_,
            element<E>* current_
        )
        : head { head_ }
        , tail { tail_ }
        , current { current_ } {}

        reference operator*() const {
            return current->backtracking_edge;
        }

        forward_walker_iterator& operator++() {
            current = current->next;
            return *this;
        }

        forward_walker_iterator operator++(int) {
            forward_walker_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        forward_walker_iterator& operator--() {
            if (current == nullptr) {
                current = tail;
            } else {
                current = current->prev;
            }
            return *this;
        }

        forward_walker_iterator operator--(int) {
            forward_walker_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const forward_walker_iterator& other) const {
            return current == other.current;
        }
    };

    static_assert(std::bidirectional_iterator<forward_walker_iterator<int>>);
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_FORWARD_WALKER_ITERATOR_H
