#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_BACKWARD_WALKER_RANGE_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_BACKWARD_WALKER_RANGE_H

#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/element.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/path_container/backward_walker_iterator.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::backward_walker_range {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::element::element;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::path_container::backward_walker_iterator
        ::backward_walker_iterator;

    template<readable_sliceable_pairwise_alignment_graph G>
    struct backward_walker_range {
    private:
        using E = typename G::E;

        element<E>* head;
        element<E>* tail;

    public:
        backward_walker_range(
            element<E>* head_,
            element<E>* tail_
        )
        : head { head_ }
        , tail { tail_ } {}

        backward_walker_iterator<G> begin() const {
            return backward_walker_iterator<G> { tail };
        }

        backward_walker_iterator<G> end() const {
            return backward_walker_iterator<G> { nullptr };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_PATH_CONTAINER_BACKWARD_WALKER_RANGE_H
