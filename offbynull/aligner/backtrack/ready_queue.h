#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/aligner/backtrack/common_allocators.h"

namespace offbynull::aligner::backtrack::ready_queue {
    template<
        typename ALLOCATOR=offbynull::aligner::backtrack::common_allocators::VectorAllocator<size_t>,
        bool error_check=true
    >
    class ready_queue {
    private:
        decltype(std::declval<ALLOCATOR>().allocate(0u)) queue;

    public:
        ready_queue(
            ALLOCATOR container_creator = {}
        ) : queue{container_creator.allocate(0u)} {
            if constexpr (error_check) {
                if (!queue.empty()) {
                    throw std::runtime_error("Queue must be sized 0 on creation");  // typically a problem when you use std::array
                }
            }
        }

        bool empty() {
            return queue.empty();
        }

        void push(size_t idx) {
            queue.push_back(idx);
        }

        size_t pop() {
            auto ret { queue.back() };
            queue.pop_back();
            return ret;
        }
    };
}
#endif //READY_QUEUE_H
