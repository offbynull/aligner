#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_UNIMPLEMENTED_SLOT_CONTAINER_CONTAINER_CREATOR_PACK_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_UNIMPLEMENTED_SLOT_CONTAINER_CONTAINER_CREATOR_PACK_H

namespace offbynull::aligner::backtrackers::graph_backtracker::slot_container::unimplemented_slot_container_container_creator_pack {
    /**
     * Unimplemented
     * @ref offbynull::aligner::backtrackers::graph_backtracker::slot_container::slot_container_container_creator_pack::slot_container_container_creator_pack,
     * intended for documentation.
     */
    struct unimplemented_slot_container_container_creator_pack {
        /**
         * Create random access container used to store slot elements.
         *
         * The behavior of this function is undefined if `begin` and `end` aren't from the same owner or if `begin > end`.
         *
         * @param begin Start iterator containing slots (elements must be initialized, where no node's parents have been walked yet).
         * @param end End iterator containing slots (elements must be initialized, where no node's parents have been walked yet).
         * @return Random access container containing elements within `[begin,end)`.
         */
        auto create_slot_container(auto begin, auto end) const;
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_GRAPH_BACKTRACKER_SLOT_CONTAINER_UNIMPLEMENTED_SLOT_CONTAINER_CONTAINER_CREATOR_PACK_H
