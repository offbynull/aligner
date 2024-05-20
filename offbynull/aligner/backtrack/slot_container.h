#ifndef SLOT_CONTAINER_H
#define SLOT_CONTAINER_H

#include <cstddef>
#include <stdexcept>
#include "offbynull/aligner/backtrack/common_allocators.h"

namespace offbynull::aligner::backtrack::slot_container {
    template<typename N, typename E>
    struct slot {
        N node;
        size_t unwalked_parent_cnt;
        E backtracking_edge;
        double backtracking_weight;
    };

    template<
        typename N,
        typename E,
        typename ALLOCATOR=offbynull::aligner::backtrack::common_allocators::VectorAllocator<slot<N, E>>
    >
    class slot_container {
    private:
        struct slots_comparator {
            bool operator()(const slot<N, E>& lhs, const slot<N, E>& rhs) const {
                return lhs.node < rhs.node;
            }

            bool operator()(const slot<N, E>& lhs, const N& rhs) const {
                return lhs.node < rhs;
            }

            bool operator()(const N& lhs, const slot<N, E>& rhs) const {
                return lhs < rhs.node;
            }
        };

        decltype(std::declval<ALLOCATOR>().allocate(0u)) slots;
    public:
        slot_container(
            auto&& begin,
            auto&& end,
            ALLOCATOR container_creator = {}
        ) : slots(container_creator.allocate(begin, end)) {
            std::ranges::sort(
                slots.begin(),
                slots.end(),
                slots_comparator{}
            );
        }
        size_t find_idx(const auto& node){
            auto it { std::lower_bound(slots.begin(), slots.end(), node, slots_comparator{}) };
            return it - slots.begin();
        }

        slot<N, E>& find_ref(const auto& node) {
            auto it { std::lower_bound(slots.begin(), slots.end(), node, slots_comparator{}) };
            return *it;
        }

        slot<N, E>& at_idx(const size_t idx) {
            return slots[idx];        }

        std::pair<size_t, slot<N, E>&> find(const auto& node) {
            auto it { std::lower_bound(slots.begin(), slots.end(), node, slots_comparator{}) };
            size_t idx { it - slots.begin() };
            slot<N, E>& slot { *it };
            return { idx, slot };
        }
    };
}

#endif //SLOT_CONTAINER_H
