#ifndef OFFBYNULL_ALIGNER_CONCEPTS_H
#define OFFBYNULL_ALIGNER_CONCEPTS_H

#include <concepts>

namespace offbynull::aligner::concepts {
    template<typename T>
    concept weight = requires(T t) {
        { t + t } -> std::same_as<T>;
        { t < t } -> std::same_as<bool>;
    };

    template<typename T>
    concept sequence = requires(T t) {
        { t[0zu] } -> std::convertible_to<auto>;  // Convertible to non-void type
        { t.size() } -> std::same_as<std::size_t>;
    };
}

#endif //OFFBYNULL_ALIGNER_CONCEPTS_H
