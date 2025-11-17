#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_BACKWARD_WALKER_ITERATOR_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_BACKWARD_WALKER_ITERATOR_H

#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include <cstdint>
#include <iterator>

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::backward_walker_iterator {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;

    /**
     * Iterator that walks the edges of an
     * @ref offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::path_container::path_container
     * in reverse.
     *
     * @tparam E Graph edge type.
     */
    template<backtrackable_edge E>
    class backward_walker_iterator {
    private:
        element<E>* current;

    public:
        // No point in documenting anything here. This is the interface for a forward iterator?

        using difference_type = std::ptrdiff_t;
        using value_type = E;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;

        backward_walker_iterator()
        : current {} {}

        backward_walker_iterator(element<E>* tail)
        : current { tail } {}

        reference operator*() const {
            return current->backtracking_edge;
        }

        backward_walker_iterator& operator++() {
            current = current->prev;
            return *this;
        }

        backward_walker_iterator operator++(int) {
            backward_walker_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const backward_walker_iterator& other) const {
            return current == other.current;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_BACKWARD_WALKER_ITERATOR_H
