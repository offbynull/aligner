#ifndef OFFBYNULL_ALIGNER_BACKTRACK_SLOT_CONTAINER_H
#define OFFBYNULL_ALIGNER_BACKTRACK_SLOT_CONTAINER_H

#include <cstddef>
#include <iterator>
#include "offbynull/aligner/backtrack/container_creator.h"
#include "offbynull/aligner/backtrack/container_creators.h"
#include "offbynull/aligner/backtrack/concepts.h"
#include "offbynull/concepts.h"

namespace offbynull::aligner::backtrack::slot_container {
    using offbynull::aligner::backtrack::concepts::weight;
    using offbynull::aligner::backtrack::container_creator::container_creator;
    using offbynull::aligner::backtrack::container_creators::vector_container_creator;
    using offbynull::concepts::input_iterator_of_type;

    template<typename N, typename E, weight WEIGHT>
    struct slot {
        N node;
        size_t unwalked_parent_cnt;
        E backtracking_edge;
        WEIGHT backtracking_weight;

        slot(N node_, size_t unwalked_parent_cnt_)
        : node{node_}
        , unwalked_parent_cnt{unwalked_parent_cnt_}
        , backtracking_edge{}
        , backtracking_weight{} {}


        slot()
        : node{}
        , unwalked_parent_cnt{}
        , backtracking_edge{}
        , backtracking_weight{} {}
    };

    template<
        typename N,
        typename E,
        weight WEIGHT,
        container_creator ALLOCATOR=vector_container_creator<slot<N, E, WEIGHT>>
    >
    class slot_container {
    private:
        struct slots_comparator {
            bool operator()(const slot<N, E, WEIGHT>& lhs, const slot<N, E, WEIGHT>& rhs) const noexcept {
                return lhs.node < rhs.node;
            }

            bool operator()(const slot<N, E, WEIGHT>& lhs, const N& rhs) const noexcept {
                return lhs.node < rhs;
            }

            bool operator()(const N& lhs, const slot<N, E, WEIGHT>& rhs) const noexcept {
                return lhs < rhs.node;
            }
        };

        decltype(std::declval<ALLOCATOR>().create_empty(std::nullopt)) slots;
    public:
        slot_container(
            input_iterator_of_type<slot<N, E, WEIGHT>> auto&& begin,
            input_iterator_of_type<slot<N, E, WEIGHT>> auto&& end,
            ALLOCATOR container_creator = {}
        ) : slots(container_creator.create_copy(begin, end)) {
            std::ranges::sort(
                slots.begin(),
                slots.end(),
                slots_comparator{}
            );
        }

        size_t find_idx(const N& node){
            auto it { std::lower_bound(slots.begin(), slots.end(), node, slots_comparator{}) };
            return it - slots.begin();
        }

        slot<N, E, WEIGHT>& find_ref(const N& node) {
            auto it { std::lower_bound(slots.begin(), slots.end(), node, slots_comparator{}) };
            return *it;
        }

        slot<N, E, WEIGHT>& at_idx(const size_t idx) {
            return slots[idx];
        }

        std::pair<size_t, slot<N, E, WEIGHT>&> find(const N& node) {
            auto it { std::lower_bound(slots.begin(), slots.end(), node, slots_comparator{}) };
            auto dist_from_beginning { std::ranges::distance(slots.begin(), it) };
            size_t idx { static_cast<size_t>(dist_from_beginning) };
            slot<N, E, WEIGHT>& slot { *it };
            return { idx, slot };
        }
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACK_SLOT_CONTAINER_H
