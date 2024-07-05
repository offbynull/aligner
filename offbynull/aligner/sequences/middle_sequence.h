#ifndef OFFBYNULL_ALIGNER_SEQUENCES_MIDDLE_SEQUENCE_H
#define OFFBYNULL_ALIGNER_SEQUENCES_MIDDLE_SEQUENCE_H

#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/sequences/prefix_sequence.h"
#include "offbynull/aligner/sequences/suffix_sequence.h"
#include <cstddef>

namespace offbynull::aligner::sequences::middle_sequence {
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::sequences::prefix_sequence::prefix_sequence;
    using offbynull::aligner::sequences::suffix_sequence::suffix_sequence;

    template<sequence SEQ, bool error_check = true>
    class middle_sequence {
    private:
        prefix_sequence<SEQ, error_check> inner_seq;
        suffix_sequence<prefix_sequence<SEQ, error_check>> seq;

    public:
        middle_sequence(SEQ& seq_, std::size_t offset, std::size_t cnt)
        : inner_seq { seq_, offset + cnt }
        , seq { inner_seq, cnt } {}

        decltype(seq[0zu]) operator[](std::size_t index) {
            return seq[index];
        }

        std::size_t size() {
            return seq.size();
        }
    };
}

#endif //OFFBYNULL_ALIGNER_SEQUENCES_MIDDLE_SEQUENCE_H
