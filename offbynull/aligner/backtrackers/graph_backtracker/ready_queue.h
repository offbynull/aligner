#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/helpers/container_creators.h"

namespace offbynull::aligner::backtrackers::graph_backtracker::ready_queue {
    using offbynull::helpers::container_creators::container_creator;
    using offbynull::helpers::container_creators::vector_container_creator;

    template<
        container_creator CONTAINER_CREATOR=vector_container_creator<std::size_t>,
        bool error_check=true
    >
    class ready_queue {
    private:
        decltype(std::declval<CONTAINER_CREATOR>().create_empty(std::nullopt)) queue;

    public:
        ready_queue(
            CONTAINER_CREATOR container_creator = {}
        ) : queue{container_creator.create_empty(std::nullopt)} {
            if constexpr (error_check) {
                if (!queue.empty()) {
                    throw std::runtime_error("Queue must be sized 0 on creation");  // Happens on std::array sized > 0
                }
            }
        }

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
#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_READY_QUEUE_H
