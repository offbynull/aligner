#include "offbynull/aligner/sequence/sequence.h"
#include "offbynull/aligner/sequences/suffix_sequence.h"
#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include <string>

namespace {
    using offbynull::aligner::sequence::sequence::sequence;
    using offbynull::aligner::sequences::suffix_sequence::suffix_sequence;

    TEST(SuffixSequenceTest, SanityTest) {
        std::string data { "hello!" };
        suffix_sequence<std::string> seq { data, 3zu };
        static_assert(sequence<decltype(seq)>);
        EXPECT_EQ(seq[0], 'l');
        EXPECT_EQ(seq[1], 'o');
        EXPECT_EQ(seq[2], '!');
    }
}
