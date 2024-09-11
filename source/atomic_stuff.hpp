#if !defined(EGT_COMPARISON_ATOMIC_STUFF_HPP)
#define EGT_COMPARISON_ATOMIC_STUFF_HPP


#include <atomic>
#include <vector>

#include "parallel_for_each.hpp"


template <typename T>
void fill(std::vector<std::atomic<T>> & atomic_vector, T value) {
    desire_parallel_for_each(atomic_vector.begin(), atomic_vector.end(),
     [&](auto & element) {
        element.store(value);
     });
}


// See https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0883r2.pdf.
template <typename T>
void initialize_if_necessary(
    std::vector<std::atomic<T>> & atomic_vector,
    T value
) {

#if defined(__cpp_lib_atomic_value_initialization)
#if 0 == __cpp_lib_atomic_value_initialization
    static bool constexpr explicit_storing_required{true};
#else
    static bool constexpr explicit_storing_required{};
#endif
#else
    static bool constexpr explicit_storing_required{true};
#endif

    if constexpr (explicit_storing_required) {
        fill(atomic_vector, value);
    }
}


#endif // EGT_COMPARISON_ATOMIC_STUFF_HPP
