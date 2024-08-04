#include "offbynull/aligner/graph/graph.h"
#include "offbynull/aligner/graphs/pairwise_local_alignment_graph.h"
#include "offbynull/aligner/graphs/pairwise_fitting_alignment_graph.h"
#include "offbynull/aligner/graphs/pairwise_overlap_alignment_graph.h"
#include "offbynull/aligner/graphs/pairwise_global_alignment_graph.h"
#include "offbynull/aligner/backtrackers/sliceable_pairwise_alignment_graph_backtracker/resident_segmenter.h"
#include "offbynull/aligner/scorers/simple_scorer.h"
#include "gtest/gtest.h"
#include <stdfloat>

namespace {
    using offbynull::aligner::backtrackers::sliceable_pairwise_alignment_graph_backtracker::resident_segmenter::resident_segmenter;
    using offbynull::aligner::graphs::pairwise_global_alignment_graph::pairwise_global_alignment_graph;
    using offbynull::aligner::graphs::pairwise_local_alignment_graph::pairwise_local_alignment_graph;
    using offbynull::aligner::graphs::pairwise_fitting_alignment_graph::pairwise_fitting_alignment_graph;
    using offbynull::aligner::graphs::pairwise_overlap_alignment_graph::pairwise_overlap_alignment_graph;
    using offbynull::aligner::scorers::simple_scorer::simple_scorer;

    TEST(BidiWalkerTest, SegmentationPointsLocal) {
        auto substitution_scorer { simple_scorer<char, char, std::float64_t>::create_substitution(1.0f64, -1.0f64) };
        auto gap_scorer { simple_scorer<char, char, std::float64_t>::create_gap(-1.0f64) };
        auto freeride_scorer { simple_scorer<char, char, std::float64_t>::create_freeride(0.0f64) };
        std::string seq1 { "aaaaalmnaaaaa" };
        std::string seq2 { "zzzzzlVnzzzzz" };
        pairwise_local_alignment_graph<decltype(seq1), decltype(seq2)> g {
            seq1,
            seq2,
            substitution_scorer,
            gap_scorer,
            freeride_scorer
        };

        resident_segmenter<decltype(g)> segmenter { g };
        using hop = decltype(segmenter)::hop;
        using segment = decltype(segmenter)::segment;
        const auto& [parts, final_weight] { segmenter.backtrack_segmentation_points() };
        std::cout << final_weight << std::endl;
        for (const auto& part : parts) {
            if (const hop* hop_ptr = std::get_if<hop>(&part)) {
                const auto& [from_node, to_node] { hop_ptr->edge.inner_edge };
                const auto& [from_node_down_offset, from_node_right_offset] { from_node };
                const auto& [to_node_down_offset, to_node_right_offset] { to_node };
                const auto& type { hop_ptr->edge.type };
                std::cout << "hop: " << from_node_down_offset << ',' << from_node_right_offset
                    << " to "
                    << to_node_down_offset << ',' << to_node_right_offset
                    << " edgetype " << static_cast<std::uint8_t>(type)
                    << std::endl;
            } else if (const segment* segment_ptr = std::get_if<segment>(&part)) {
                const auto& [from_node_down_offset, from_node_right_offset] { segment_ptr->from_node };
                const auto& [to_node_down_offset, to_node_right_offset] { segment_ptr->to_node };
                std::cout << "segment: " << from_node_down_offset << ',' << from_node_right_offset
                    << " to "
                    << to_node_down_offset << ',' << to_node_right_offset
                    << std::endl;
            } else {
                throw std::runtime_error("This should never hapen");
            }
        }
    }
}
