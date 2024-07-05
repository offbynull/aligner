#ifndef OFFBYNULL_ALIGNER_SEQUENCES_REVERSED_SEQUENCE_H
#define OFFBYNULL_ALIGNER_SEQUENCES_REVERSED_SEQUENCE_H

#include "offbynull/aligner/sequence/sequence.h"
#include <cstddef>

namespace offbynull::aligner::sequences::reversed_sequence {
    using offbynull::aligner::sequence::sequence::sequence;

    template<sequence SEQ, bool error_check = true>
    class reversed_sequence {
    private:
        SEQ& seq;

    public:
        reversed_sequence(SEQ& seq_)
        : seq { seq_ } { }

        decltype(seq[0zu]) operator[](std::size_t index) {
            return seq[seq.size() - index - 1zu];
        }

        std::size_t size() {
            return seq.size();
        }
    };
}

#endif //OFFBYNULL_ALIGNER_SEQUENCES_REVERSED_SEQUENCE_H
