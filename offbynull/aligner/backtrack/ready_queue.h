#ifndef OFFBYNULL_ALIGNER_BACKTRACK_READY_QUEUE_H
#define OFFBYNULL_ALIGNER_BACKTRACK_READY_QUEUE_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/aligner/backtrack/allocator.h"
#include "offbynull/aligner/backtrack/allocators.h"

namespace offbynull::aligner::backtrack::ready_queue {
    using offbynull::aligner::backtrack::allocator::allocator;
    using offbynull::aligner::backtrack::allocators::VectorAllocator;

    template<
        allocator ALLOCATOR=VectorAllocator<size_t>,
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
                    throw std::runtime_error("Queue must be sized 0 on creation");  // Happens on std::array sized > 0
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
#endif //OFFBYNULL_ALIGNER_BACKTRACK_READY_QUEUE_H
