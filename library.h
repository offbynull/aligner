#ifndef LIBRARY_H
#define LIBRARY_H

#include "offbynull/aligner/backtrack/backtrack.h"
#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"

template<typename ELEM, typename ED>
void global_align(
    const auto& range1,
    const auto& range2,
    std::function<ED(const std::optional<std::reference_wrapper<const ELEM>>&, const std::optional<std::reference_wrapper<const ELEM>>&)> weight_lookup
) {

}

extern "C" {
    int add(int a, int b) {

    }
}

#endif //LIBRARY_H
