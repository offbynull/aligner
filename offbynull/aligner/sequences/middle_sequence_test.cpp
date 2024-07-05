#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/sequences/middle_sequence.h"
#include "gtest/gtest.h"
#include <string>

namespace {
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::sequences::middle_sequence::middle_sequence;

    TEST(MiddleSequenceTest, SanityTest) {
        std::string data { "hello!" };
        middle_sequence<std::string> seq { data, 1zu, 3zu };
        static_assert(sequence<decltype(seq)>);
        EXPECT_EQ(seq[0], 'e');
        EXPECT_EQ(seq[1], 'l');
        EXPECT_EQ(seq[2], 'l');
    }
}
