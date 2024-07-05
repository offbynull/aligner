#ifndef OFFBYNULL_ALIGNER_SEQUENCES_SUFFIX_SEQUENCE_H
#define OFFBYNULL_ALIGNER_SEQUENCES_SUFFIX_SEQUENCE_H

#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/sequences/reversed_sequence.h"
#include "offbynull/aligner/sequences/prefix_sequence.h"
#include <cstddef>

namespace offbynull::aligner::sequences::suffix_sequence {
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::sequences::reversed_sequence::reversed_sequence;
    using offbynull::aligner::sequences::prefix_sequence::prefix_sequence;

    template<sequence SEQ, bool error_check = true>
    class suffix_sequence {
    private:
        reversed_sequence<SEQ, error_check> inner_inner_seq;
        prefix_sequence<reversed_sequence<SEQ, error_check>> inner_seq;
        reversed_sequence<prefix_sequence<reversed_sequence<SEQ, error_check>>> seq;

    public:
        suffix_sequence(SEQ& seq_, std::size_t suffix_size_)
        : inner_inner_seq { seq_ }
        , inner_seq { inner_inner_seq, suffix_size_ }
        , seq { inner_seq } {}

        decltype(seq[0zu]) operator[](std::size_t index) const {
            return seq[index];
        }

        std::size_t size() const {
            return seq.size();
        }
    };
}

#endif //OFFBYNULL_ALIGNER_SEQUENCES_SUFFIX_SEQUENCE_H
