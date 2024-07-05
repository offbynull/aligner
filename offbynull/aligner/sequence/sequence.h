#ifndef OFFBYNULL_ALIGNER_SEQUENCE_SEQUENCE_H
#define OFFBYNULL_ALIGNER_SEQUENCE_SEQUENCE_H

#include <concepts>

namespace offbynull::aligner::sequence::sequence {
    template<typename T>
    concept sequence_element = std::regular<std::decay_t<T>>;

    template<typename T>
    concept sequence = requires(T t) {
        { t[0zu] } -> sequence_element;
        { t[0zu] } -> std::convertible_to<auto>;  // Convertible to non-void type
        { t.size() } -> std::same_as<std::size_t>;
    };
}

#endif //OFFBYNULL_ALIGNER_SEQUENCE_SEQUENCE_H
