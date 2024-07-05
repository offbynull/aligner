#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/sequences/prefix_sequence.h"
#include "gtest/gtest.h"
#include <string>

namespace {
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::sequences::prefix_sequence::prefix_sequence;

    TEST(PrefixSequenceTest, SanityTest) {
        std::string data { "hello!" };
        prefix_sequence<std::string> seq { data, 3zu };
        static_assert(sequence<decltype(seq)>);
        EXPECT_EQ(seq[0], 'h');
        EXPECT_EQ(seq[1], 'e');
        EXPECT_EQ(seq[2], 'l');
    }
}
