#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_CONTAINER_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_CONTAINER_H

#include <functional>
#include <ranges>
#include <algorithm>
#include <vector>
#include <utility>
#include <optional>
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_container_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_container_heap_container_creator_pack.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_with_node.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_slot_container/resident_slot_with_node_comparator.h"

namespace offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container {
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_container_creator_pack::resident_slot_container_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_container_heap_container_creator_pack::resident_slot_container_heap_container_creator_pack;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container::resident_slot_with_node
        ::resident_slot_with_node;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot_with_node_comparator::resident_slot_with_node_comparator;
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_slot_container
        ::resident_slot::resident_slot;

    template<
        bool debug_mode,
        readable_sliceable_pairwise_alignment_graph G,
        resident_slot_container_container_creator_pack<
            typename G::N,
            typename G::E,
            typename G::ED
        > CONTAINER_CREATOR_PACK = resident_slot_container_heap_container_creator_pack<
            debug_mode,
            typename G::N,
            typename G::E,
            typename G::ED,
            true
        >
    >
    class resident_slot_container {
    private:
        using N = typename G::N;
        using E = typename G::E;
        using ND = typename G::ND;
        using ED = typename G::ED;
        using INDEX = typename G::INDEX;
        using SLOT_CONTAINER = decltype(
            std::declval<CONTAINER_CREATOR_PACK>().create_slot_container(
                std::declval<std::vector<resident_slot_with_node<N, E, ED>>>()
            )
        );

        SLOT_CONTAINER slots;

    public:
        resident_slot_container(
            const G& g,
            CONTAINER_CREATOR_PACK container_creator_pack = {}
        )
        : slots {
            container_creator_pack.create_slot_container(
                g.resident_nodes()
                | std::views::transform([](const N& node) {
                    return resident_slot_with_node<N, E, ED> {
                        node,
                        {}
                    };
                })
            )
        } {
            std::ranges::sort(slots.begin(), slots.end(), resident_slot_with_node_comparator<N, E, ED> {});
        }

        std::optional<std::reference_wrapper<resident_slot<E, ED>>> find(const N& node) {
            auto it {
                std::lower_bound(
                    slots.begin(),
                    slots.end(),
                    node,
                    resident_slot_with_node_comparator<N, E, ED> {}
                )
            };
            if (it != slots.end() && (*it).node == node) {
                return { (*it).slot_ };
            }
            return { std::nullopt };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_RESIDENT_SLOT_CONTAINER_RESIDENT_SLOT_CONTAINER_H
