#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H

#include <cstddef>
#include "offbynull/concepts.h"
#include "offbynull/utils.h"
#include "offbynull/aligner/concepts.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/backtrackable_edge.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_with_node.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_stack_container_creator_pack {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_with_node
        ::resident_slot_with_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_node::backtrackable_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::backtrackable_edge::backtrackable_edge;
    using offbynull::aligner::concepts::weight;
    using offbynull::concepts::range_of_type;
    using offbynull::utils::static_vector_typer;

    template<
        bool debug_mode,
        backtrackable_node N,
        backtrackable_edge E,
        weight ED,
        std::size_t resident_nodes_capacity
    >
    struct resident_slot_container_stack_container_creator_pack {
        using CONTAINER_TYPE = typename static_vector_typer<
            debug_mode,
            resident_slot_with_node<N, E, ED>,
            resident_nodes_capacity
        >::type;
        CONTAINER_TYPE create_slot_container(range_of_type<resident_slot_with_node<N, E, ED>> auto&& r) const  {
            return CONTAINER_TYPE(r.begin(), r.end());
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_CONTAINER_STACK_CONTAINER_CREATOR_PACK_H
