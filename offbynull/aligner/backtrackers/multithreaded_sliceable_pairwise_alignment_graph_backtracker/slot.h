#ifndef OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_H
#define OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_H

#include <optional>
#include "offbynull/aligner/concepts.h"
#include "offbynull/utils.h"

namespace offbynull::aligner::backtrackers::multithreaded_sliceable_pairwise_alignment_graph_backtracker::slot {
    using offbynull::aligner::concepts::weight;
    using offbynull::utils::packable_optional;

    PACK_STRUCT_START
    template<typename E, weight WEIGHT>
    struct slot {
        packable_optional<E> backtracking_edge;
        WEIGHT backtracking_weight;

        slot(E backtracking_edge_, WEIGHT backtracking_weight_)
        : backtracking_edge { backtracking_edge_ }
        , backtracking_weight { backtracking_weight_ } {}

        slot()
        : backtracking_edge { std::nullopt }
        , backtracking_weight {} {}

        bool operator==(const slot&) const = default;
    }
    PACK_STRUCT_STOP;
}

#endif //OFFBYNULL_ALIGNER_BACKTRACKERS_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_BACKTRACKER_SLOT_H
