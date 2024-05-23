#ifndef OFFBYNULL_CONCEPTS_H
#define OFFBYNULL_CONCEPTS_H

#include <cstddef>
#include <ranges>

namespace offbynull::concepts {
    template <typename T, typename V>
    concept random_access_range_of_type = std::ranges::random_access_range<T> && std::same_as<std::ranges::range_reference_t<T>, V&>;

    template <typename T, typename V>
    concept input_iterator_of_type = std::input_iterator<T> && std::same_as<typename T::value_type, V>;

    template<typename T, typename V>
    concept range_of_type = std::ranges::range<T> && std::same_as<std::ranges::range_value_t<T>, V>;

    template<typename T, typename... Ts>
    concept range_of_one_of = std::ranges::range<T> && (std::same_as<std::ranges::range_value_t<T>, Ts> || ...);

    template<typename T, typename ... Ts>
    concept one_of = (std::same_as<T, Ts> || ...);

    // template(s) to extract parameter types
    template <std::size_t N, typename T0, typename ... Ts>
    struct typeN { using type = typename typeN<N-1U, Ts...>::type; };

    template <typename T0, typename ... Ts>
    struct typeN<0U, T0, Ts...> { using type = T0; };

    template <std::size_t, typename F>
    struct argN;

    template <std::size_t N, typename R, typename ... As>
    struct argN<N, R(*)(As...)> { using type = typename typeN<N, As...>::type; };

    template <std::size_t N, typename R, typename ... As>
    struct argN<N, R(As...)>  { using type = typename typeN<N, As...>::type; };
}

#endif //OFFBYNULL_CONCEPTS_H
