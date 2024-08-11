#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_H

#include <cstddef>
#include "offbynull/helpers/container_creators.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::pairwise_alignment_graph_backtracker::ready_queue {
    using offbynull::aligner::graph::pairwise_alignment_graph::readable_pairwise_alignment_graph;
    using offbynull::concepts::random_access_range_of_type;
    using offbynull::utils::static_vector_typer;




    template<
        typename T,
        typename G
    >
    concept ready_queue_container_creator_pack =
    readable_pairwise_alignment_graph<G>
    && requires(T t) {
        { t.create_queue_container() } -> random_access_range_of_type<std::size_t>;
    };

    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G
    >
    struct ready_queue_heap_container_creator_pack {
        using N = typename G::N;
        using E = typename G::E;

        std::vector<std::size_t> create_queue_container() {
            return std::vector<std::size_t> {};
        }
    };

    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G,
        std::size_t grid_down_cnt,
        std::size_t grid_right_cnt
    >
    struct ready_queue_stack_container_creator_pack {
        using N = typename G::N;
        using E = typename G::E;

        static constexpr std::size_t ELEM_COUNT { grid_down_cnt * grid_right_cnt * G::limits(grid_down_cnt, grid_right_cnt).max_grid_node_depth };
        using CONTAINER_TYPE = typename static_vector_typer<std::size_t, ELEM_COUNT, debug_mode>::type;

        CONTAINER_TYPE create_queue_container() {
            return CONTAINER_TYPE {};
        }
    };
    
    
    
    
    template<
        bool debug_mode,
        readable_pairwise_alignment_graph G,
        ready_queue_container_creator_pack<G> CONTAINER_CREATOR_PACK=ready_queue_heap_container_creator_pack<debug_mode, G>
    >
    class ready_queue {
    private:
        using QUEUE_CONTAINER=decltype(std::declval<CONTAINER_CREATOR_PACK>().create_queue_container());

        QUEUE_CONTAINER queue;

    public:
        ready_queue(
            CONTAINER_CREATOR_PACK container_creator_pack = {}
        )
        : queue{container_creator_pack.create_queue_container()} {}

        bool empty() {
            return queue.empty();
        }

        void push(std::size_t idx) {
            queue.push_back(idx);
        }

        std::size_t pop() {
            auto ret { queue.back() };
            queue.pop_back();
            return ret;
        }
    };
}
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_READY_QUEUE_H
