#ifndef OFFBYNULL_ALIGNER_SEQUENCES_PREFIX_SEQUENCE_H
#define OFFBYNULL_ALIGNER_SEQUENCES_PREFIX_SEQUENCE_H

#include "offbynull/aligner/sequence/sequence.h"
#include <cstddef>
#include <stdexcept>

namespace offbynull::aligner::sequences::prefix_sequence {
    using offbynull::aligner::sequence::sequence::sequence;

    template<sequence SEQ, bool error_check = true>
    class prefix_sequence {
    private:
        SEQ& seq;
        std::size_t prefix_size;

    public:
        prefix_sequence(SEQ& seq_, std::size_t prefix_size_)
        : seq { seq_ }
        , prefix_size { prefix_size_ } {
            if constexpr (error_check) {
                if (prefix_size > seq_.size()) {
                    throw std::runtime_error("Prefix size exceeds actual size");
                }
            }
        }

        decltype(seq[0zu]) operator[](std::size_t index) const {
            return seq[index];
        }

        std::size_t size() const {
            return prefix_size;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_SEQUENCES_PREFIX_SEQUENCE_H
