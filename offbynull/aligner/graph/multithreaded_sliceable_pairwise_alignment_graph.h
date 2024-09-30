#ifndef OFFBYNULL_ALIGNER_GRAPH_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_H
#define OFFBYNULL_ALIGNER_GRAPH_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_H

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <tuple>
#include <algorithm>
#include "offbynull/concepts.h"
#include "offbynull/helpers/join_bidirectional_view.h"
#include "offbynull/aligner/graph/sliceable_pairwise_alignment_graph.h"

namespace offbynull::aligner::graph::multithreaded_sliceable_pairwise_alignment_graph {
    using offbynull::concepts::bidirectional_range_of_bidirectional_range_of_one_of;
    using offbynull::concepts::unqualified_value_type;
    using offbynull::helpers::join_bidirectional_view::join_bidirectional_view_adaptor;
    using offbynull::aligner::graph::sliceable_pairwise_alignment_graph::readable_sliceable_pairwise_alignment_graph;

    enum class axis : std::uint8_t {
        DOWN_FROM_TOP_LEFT = 0u,
        RIGHT_FROM_BOTTOM_LEFT = 1u
    };

    template <typename G>
    concept readable_multithreaded_sliceable_pairwise_alignment_graph =
        unqualified_value_type<G>
        && readable_sliceable_pairwise_alignment_graph<G>
        && requires(
            G g,
            typename G::N node,
            axis axis_,
            typename G::INDEX axis_position,
            std::size_t max_segments
        ) {
            { g.segmented_diagonal_nodes(axis_, axis_position, max_segments) }
                -> bidirectional_range_of_bidirectional_range_of_one_of<typename G::N, const typename G::N&>;
            { g.segmented_diagonal_nodes(axis_, axis_position, node, node, max_segments) }
                -> bidirectional_range_of_bidirectional_range_of_one_of<typename G::N, const typename G::N&>;
        };




    // Reference implementation for segmented_diagonal_nodes()
    // -------------------------------------------------------
    // You can either ...
    // 1. have the readable_sliceable_pairwise_alignment_graph's segmented_diagonal_nodes() function call into this function
    // 2. write a custom implementation for the readable_sliceable_pairwise_alignment_graph's segmented_diagonal_nodes() (typically more
    //    optimized than) and test against this to ensure things are working correctly.
    template<
        bool debug_mode,
        typename G  /* Should be readable_multithreaded_sliceable_pairwise_alignment_graph, but can't do this because of cyclic dep. */
    >
    std::ranges::bidirectional_range auto generic_segmented_diagonal_nodes(
        const G& g,
        axis axis_,
        typename G::INDEX axis_position,
        const typename G::N& root_node,
        const typename G::N& leaf_node,
        std::size_t max_segments
    ) {
        using INDEX = typename G::INDEX;
        using N = typename G::N;

        if constexpr (debug_mode) {
            if (!g.has_node(root_node) || !g.has_node(leaf_node)) {
                throw std::runtime_error { "Bad root / leaf node " };
            }
            switch (axis_) {
                case axis::DOWN_FROM_TOP_LEFT:
                    if (!(axis_position >= root_node.down && axis_position <= leaf_node.down)) {
                        throw std::runtime_error { "Bad node" };
                    }
                break;
                case axis::RIGHT_FROM_BOTTOM_LEFT:
                    if (!(axis_position >= root_node.right && axis_position <= leaf_node.right)) {
                        throw std::runtime_error { "Bad node" };
                    }
                break;
                [[unlikely]] default:
                    throw std::runtime_error { "This should never happen" };
            }
        }

        /*
                          0
                          1 1
         0 1 2 3 4        2 2 2
         1 2 3 4 5 1      3 3 3
         2 3 4 5 6        4 4 4
               3          5 5
                          6


                          0
         0 1 2            1 1
         1 2 3            2 2 2
         2 3 4            3 3 3
         3 4 5            4 4 4
         4 5 6            5 5
                          6
         */
        std::pair<INDEX, INDEX> grid_start;
        std::pair<INDEX, INDEX> grid_stop;  // inclusive
        switch (axis_) {
            case axis::DOWN_FROM_TOP_LEFT: {
                grid_start = { axis_position, 0u };
                if (axis_position < g.grid_right_cnt) {
                    grid_stop = { 0u, axis_position };
                } else {
                    grid_stop = { axis_position - (g.grid_right_cnt - 1u), g.grid_right_cnt - 1u };
                }
                break;
            }
            case axis::RIGHT_FROM_BOTTOM_LEFT: {
                grid_start = { g.grid_down_cnt - 1u, axis_position };
                if (g.grid_down_cnt < g.grid_right_cnt) {
                    if (axis_position < g.grid_down_cnt) {
                        grid_stop = { 0u, axis_position + g.grid_right_cnt - 1u };  // ok
                    } else {
                        grid_stop = { axis_position - g.grid_down_cnt + 1u, g.grid_right_cnt - 1u };  // ok
                    }
                } else {
                    grid_stop = { g.grid_down_cnt - (g.grid_right_cnt - axis_position), g.grid_right_cnt - 1u }; // ok
                }
                break;
            }
            [[unlikely]] default: {
                if constexpr (debug_mode) {
                    throw std::runtime_error { "This should never happen" };
                }
                std::unreachable();
            }
        }

        // grid_start's down will always be below or at that of grid_stop's down, so no need to worry about rollover for diag_len.
        INDEX diag_len { std::get<0>(grid_start) - std::get<0>(grid_stop) + 1u };
        INDEX segment_cnt { std::min(max_segments, diag_len) };
        INDEX inc_per_segment { diag_len / segment_cnt };
        return
            std::views::iota(0zu, segment_cnt)
            | std::views::transform(
                [&g, root_node, leaf_node, grid_start, grid_stop, inc_per_segment, segment_cnt](const auto& segment) {
                    // First grid offset in segment
                    std::pair<INDEX, INDEX> segment_first {
                        std::get<0>(grid_start) + (segment * inc_per_segment),
                        std::get<1>(grid_start) + (segment * inc_per_segment)
                    };
                    // Last grid offset in segment
                    std::pair<INDEX, INDEX> segment_last;
                    if (segment != segment_cnt - 1u) {
                        segment_last = {
                            std::get<0>(segment_first) + (segment * inc_per_segment) - 1u,  // -1 because inclusive
                            std::get<1>(segment_first) + (segment * inc_per_segment) - 1u   // -1 because inclusive
                        };
                    } else {
                        // Ensure last segment consumes the remainder -- division used to produce inc_per_segment may have resulted in
                        // rounding.
                        segment_last = grid_stop;
                    }
                    // Return range that walks between the two (inclusive)
                    const auto& [segment_first_down, segment_first_right] { segment_first };
                    const auto& [segment_last_down, segment_last_right] { segment_last };
                    INDEX steps { segment_first_down - segment_last_down + 1u };
                    return
                        std::views::iota(0zu, steps)
                        | std::views::transform(
                            [&g, segment_first_down, segment_first_right](const INDEX& step) {
                                return g.grid_offset_to_nodes(
                                    segment_first_down - step,
                                    segment_first_right + step
                                );
                            }
                        )
                        | join_bidirectional_view_adaptor {}
                        | std::views::filter(
                            [&g, root_node, leaf_node](const N& node) {
                                return g.is_reachable(root_node, node) && g.is_reachable(node, leaf_node);
                            }
                        );
                }
            );
    }

    template<
        bool debug_mode,
        typename G  /* Should be readable_multithreaded_sliceable_pairwise_alignment_graph, but can't do this because of cyclic dep. */
    >
    std::ranges::bidirectional_range auto generic_segmented_diagonal_nodes(
        const G& g,
        axis axis_,
        typename G::INDEX axis_position,
        std::size_t max_segments
    ) {
        return generic_segmented_diagonal_nodes<debug_mode, G>(
            g,
            axis_,
            axis_position,
            g.get_root_node(),
            g.get_leaf_node(),
            max_segments
        );
    }
}

#endif //OFFBYNULL_ALIGNER_GRAPH_MULTITHREADED_SLICEABLE_PAIRWISE_ALIGNMENT_GRAPH_H
