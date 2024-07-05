#ifndef OFFBYNULL_ALIGNER_SEQUENCES_PREFIX_PAD_SEQUENCE_H
#define OFFBYNULL_ALIGNER_SEQUENCES_PREFIX_PAD_SEQUENCE_H

#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/utils.h"
#include <cstddef>
#include <stdexcept>

namespace offbynull::aligner::sequences::suffix_pad_sequence {
    using offbynull::aligner::sequence::sequence::sequence;

    template<sequence SEQ, bool error_check = true>
    class suffix_pad_sequence {
    private:
        using SEQ_ELEM_RET_TYPE = decltype(std::declval<SEQ>()[0zu]);
        using SEQ_ELEM = std::remove_reference_t<SEQ_ELEM_RET_TYPE>;

        SEQ& seq;
        SEQ_ELEM pad_value;
        std::size_t pad_count;

    public:
        suffix_pad_sequence(
            SEQ& seq_,
            SEQ_ELEM pad_value_,
            std::size_t pad_count_
        )
        : seq { seq_ }
        , pad_value { pad_value_ }
        , pad_count { pad_count_ } {}

        SEQ_ELEM_RET_TYPE operator[](std::size_t index) {
            if (index >= seq.size()) {
                return pad_value;
            }
            return seq[index];
        }

        std::size_t size() const {
            return seq.size() + pad_count;
        }
    };
}

#endif //OFFBYNULL_ALIGNER_SEQUENCES_PREFIX_PAD_SEQUENCE_H
