#ifndef EGT_COMPARISON_PARALLEL_FOR_EACH_HPP
#define EGT_COMPARISON_PARALLEL_FOR_EACH_HPP


/*
The purpose of the desire_parallel_for_each() template function is to use the
std::execution::par feature when it is available, without preventing successful
compilation, linking, and running when it is not available.

C++17 added a std::for_each overload that accepts an execution policy.  This
standard library feature is supported by g++ versions >= 10 with libstdc++, but
only when -fexceptions is enabled.  This standard library feature is also
supported by clang++ version 14 with libstdc++.  Use with clang++ does not
require that -fexceptions be enabled.

N.B.  We have switched away from using the libc++ standard library
implementation that ships with clang++ version 14, which does not provide the
overload that accepts an execution policy at all.
*/


#include <algorithm>


#if defined(__clang__) && (14 <= __clang_major__)
#    define EGT_COMPARISON_COMPILER_SUPPORTS_EXECUTION_POLICIES 1
#elif defined(__GNUC__) && (10 <= __GNUC__) && defined (__EXCEPTIONS) && (1 == __EXCEPTIONS)
#    define EGT_COMPARISON_COMPILER_SUPPORTS_EXECUTION_POLICIES 1
#else
#    undef EGT_COMPARISON_COMPILER_SUPPORTS_EXECUTION_POLICIES
#endif

#if defined _LIBCPP_VERSION
#    undef EGT_COMPARISON_STANDARD_LIBRARY_SUPPORTS_EXECUTION_POLICIES
#else
#    define EGT_COMPARISON_STANDARD_LIBRARY_SUPPORTS_EXECUTION_POLICIES 1
#endif

#if defined(EGT_COMPARISON_COMPILER_SUPPORTS_EXECUTION_POLICIES) && defined (EGT_COMPARISON_STANDARD_LIBRARY_SUPPORTS_EXECUTION_POLICIES)
#    define EGT_COMPARISON_PARALLEL_FOR_EACH_AVAILABLE 1
#else
#    undef EGT_COMPARISON_PARALLEL_FOR_EACH_AVAILABLE
#endif

#if defined(EGT_COMPARISON_PARALLEL_FOR_EACH_AVAILABLE)
#    include <execution>
#endif


bool constexpr parallel_for_each_is_available() {
#if defined(EGT_COMPARISON_PARALLEL_FOR_EACH_AVAILABLE)
    return true;
#else
    return false;
#endif
};


template <
    typename fwd_iter_type,
    typename move_constructible_fn_type
> void serial_for_each(
    fwd_iter_type first,
    fwd_iter_type past_last,
    move_constructible_fn_type unary_fn
) {
    std::for_each(first, past_last, unary_fn);
};


template <
    typename fwd_iter_type,
    typename copy_constructible_fn_type
> void desire_parallel_for_each(
    fwd_iter_type first,
    fwd_iter_type past_last,
    copy_constructible_fn_type unary_fn
) {
#if defined(EGT_COMPARISON_PARALLEL_FOR_EACH_AVAILABLE)
    std::for_each(std::execution::par, first, past_last, unary_fn);
#else
    // Copy-constructible implies move-constructible.
    serial_for_each(first, past_last, unary_fn);
#endif
};


template <
    typename fwd_iter_type,
    typename copy_constructible_fn_type
> void optionally_parallel_for_each(
    bool const use_parallelism_if_available,
    fwd_iter_type first,
    fwd_iter_type past_last,
    copy_constructible_fn_type unary_fn
) {
    if (use_parallelism_if_available) {
        desire_parallel_for_each(first, past_last, unary_fn);
    } else {
        // Copy-constructible implies move-constructible.
        serial_for_each(first, past_last, unary_fn);
    }
}


#endif // EGT_COMPARISON_PARALLEL_FOR_EACH_HPP
