#if !defined (EGT_COMPARISON_INITIALIZE_WITH_HPP)
#define EGT_COMPARISON_INITIALIZE_WITH_HPP


#include <array>
#include <utility>


template <typename type, typename... value_types>
std::array<type, sizeof...(value_types)> constexpr
initialize_with(value_types && ... args) noexcept {
    return {static_cast<type>(std::forward<value_types>(args))...};
}


#endif // EGT_COMPARISON_INITIALIZE_WITH_HPP
