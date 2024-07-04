TODO:
* There's a huge influx of "container_creator" type parameters -- find a better way to package this up?
* ~~Fix include guards to be full path, not just filename~~
* ~~Change all occurrences of size_t to std::size_t~~
* ~~Use std::unreachable() any place with known unreachable?~~
* ~~Remove underscores from template parameters because they're causing warnings.~~
* ~~Add function for stack 8x8, 16x16, etc..~~
* ~~Change references of input_iterator to forward_iterator~~
  * ~~When you change this, you may be able to swap out some of your custom implementations of stuff (in utils.h) to standard stl funcs~~

* ~~UPDATE ALIGNMENT GRAPHS SO YOU PASS THE SEQUENCES DIRECTLY TO GRAPH~~

  ```c++
    using empty_type = std::tuple<>;

    template<
        std::ranges::random_access_range DOWN_SEQ,
        std::ranges::random_access_range RIGHT_SEQ,
        widenable_to_size_t INDEX_ = unsigned int,
        weight WEIGHT = std::float64_t,
        bool error_check = true
    >
    class pairwise_global_alignment_graph {
    public:
        using INDEX = INDEX_;
        using N = std::pair<INDEX, INDEX>;
        using E = std::pair<N, N>;
        using ND = empty_type;
        using ED = WEIGHT;  // Differs from backing grid_graph because these values are derived at time of access

    private:
        grid_graph<
            empty_type,
            empty_type,
            INDEX,
            vector_grid_container_creator<empty_type, INDEX_, false>,
            vector_grid_container_creator<empty_type, INDEX_, false>,
            error_check
        > g;
        const DOWN_SEQ& down_seq;
        const RIGHT_SEQ& right_seq;
        std::function<
            WEIGHT(
                INDEX,
                const std::optional<std::reference_wrapper<const std::decay_t<decltype(down_seq[0u])>>>&,
                INDEX,
                const std::optional<std::reference_wrapper<const std::decay_t<decltype(right_seq[0u])>>>&
            )
        > match_lookup;
        std::function<
            WEIGHT(
                INDEX,
                INDEX
            )
        > indel_lookup;

    public:
        const INDEX grid_down_cnt;
        const INDEX grid_right_cnt;

        pairwise_global_alignment_graph(
            const DOWN_SEQ& _down_seq,
            const RIGHT_SEQ& _right_seq,
            decltype(match_lookup) _match_lookup,
            decltype(indel_lookup) _indel_lookup
        )
        : g{_down_seq.size(), _right_seq.size(), {}, {}, {}}
        , down_seq{_down_seq}
        , right_seq{_right_seq}
        , grid_down_cnt{_down_seq.size()}
        , grid_right_cnt{_right_seq.size()} {}
  ```

* Update get_in_degree() / get_out_degree() functions to calculate directly
* Add concept checks to autos
* Add random access container wrapper that pads out with dummy (or truncates) to a certain size
* Add random access container wrapper that mmap (it's in boost)
* Add random access container wrapper that decompresses as you read
* Add divide-and-conquer aligner interface methods and algorithm

  ```c++
  constexpr std::size_t G::max_slice_predecessor_count; // make number of predecessors required by a slice
  auto slice_walk(INDEX column); // returns a slice in order of dependencies
  auto slice_contants();  // returns node weights that MUST be always be kept in-memory (in addition to slice_walk(i-1)
  
  class pairwise_reversed_graph;  // graph reverses a pairwise graph's edges, so that leaf is root and vice versa.
  class pairwise_graph_view;      // graph that limits the down/right node counts
  ```

* Integer promotion rules hit when you use types smaller than int:
  * e.g. subtracting two chars results in an int, meaning if you have char x = c1 - c2, you'll get "warning: conversion from ‘int’ to ‘unsigned char’ may change value [-Warith-conversion]"
  * Lots of places +1u -1u or == 0u, use offbynull::utils::constants<T> to get literals of the correct type ()
  * option 1: you might have to forcefully static_cast every addition / subtraction?
  * option 2: you might have to write a integer wrapper that avoids integer promotion rules (internally static_casts)? 