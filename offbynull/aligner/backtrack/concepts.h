#ifndef OFFBYNULL_ALIGNER_BACKTRACK_CONCEPTS_H
#define OFFBYNULL_ALIGNER_BACKTRACK_CONCEPTS_H

#include <concepts>

namespace offbynull::aligner::backtrack::concepts {
    template<typename T>
    concept weight = requires(T t) {
        { t + t } -> std::same_as<T>;
        { t < t } -> std::same_as<bool>;
    };
}

#endif //OFFBYNULL_ALIGNER_BACKTRACK_CONCEPTS_H
