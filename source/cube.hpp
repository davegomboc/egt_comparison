#ifndef EGT_COMPARISON_CUBE_HPP
#define EGT_COMPARISON_CUBE_HPP


#include <cstddef>

#include "cube_binary_inputs.hpp"
#include "uint_type_selection.hpp"


template <
    std::size_t const required_input_bit_pair_count_v,
    std::size_t const required_output_bit_count_v
> class cube_tt {
public:
    static std::size_t constexpr required_input_bit_pair_count{
     required_input_bit_pair_count_v};
    static std::size_t constexpr required_output_bit_count{
     required_output_bit_count_v};

    using inputs_type = cube_binary_inputs_tt<required_input_bit_pair_count>;
    static_assert(required_input_bit_pair_count
     == inputs_type::required_bit_pair_count);

    using inputs_value_type = typename inputs_type::value_type;
    using inputs_index_type = typename inputs_type::index_type;
    using outputs_value_type = selected_uint_tt<required_output_bit_count>;

    static std::size_t constexpr input_bit_count{
     inputs_type::value_type_bit_count};

    cube_tt & operator=(cube_tt const &) = default;
    cube_tt & operator=(cube_tt &&) = default;
    cube_tt(cube_tt const &) = default;
    cube_tt(cube_tt &&) = default;
    ~cube_tt() = default;
    cube_tt() = default;

    template <typename ITYPE, typename OTYPE>
    cube_tt(ITYPE && i, OTYPE && o)
      : the_inputs(std::forward<ITYPE>(i)),
        the_outputs(std::forward<OTYPE>(o)) {}

    inputs_type const & inputs() const {
        return the_inputs;
    }
    inputs_type & inputs() {
        return the_inputs;
    }
    outputs_value_type const & outputs() const {
        return the_outputs;
    }
    outputs_value_type & outputs() {
        return the_outputs;
    }

private:
    inputs_type the_inputs;
    outputs_value_type the_outputs;
};

template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> bool operator==(
    cube_tt<input_bit_pair_count, output_bit_count> const & lhs,
    cube_tt<input_bit_pair_count, output_bit_count> const & rhs
) {
    return (lhs.inputs() == rhs.inputs()) and (lhs.outputs() == rhs.outputs());
}

template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> bool operator!=(
    cube_tt<input_bit_pair_count, output_bit_count> const & lhs,
    cube_tt<input_bit_pair_count, output_bit_count> const & rhs
) {
    return not (lhs == rhs);
}

template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> bool operator<(
    cube_tt<input_bit_pair_count, output_bit_count> const & lhs,
    cube_tt<input_bit_pair_count, output_bit_count> const & rhs
) {
    return (lhs.inputs() < rhs.inputs())
     or ((lhs.inputs() == rhs.inputs()) and (lhs.outputs() < rhs.outputs()));
}


template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> bool operator>(
    cube_tt<input_bit_pair_count, output_bit_count> const & lhs,
    cube_tt<input_bit_pair_count, output_bit_count> const & rhs
) {
    return rhs < lhs;
}


template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> bool operator<=(
    cube_tt<input_bit_pair_count, output_bit_count> const & lhs,
    cube_tt<input_bit_pair_count, output_bit_count> const & rhs
) {
    return not (lhs > rhs);
}


template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> bool operator>=(
    cube_tt<input_bit_pair_count, output_bit_count> const & lhs,
    cube_tt<input_bit_pair_count, output_bit_count> const & rhs
) {
    return not (lhs < rhs);
}


template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> std::ostream & operator<<(
    std::ostream & out,
    cube_tt<input_bit_pair_count, output_bit_count> const & cube
) {
    using outputs_value_type = typename
     cube_tt<input_bit_pair_count, output_bit_count>::outputs_value_type;
    out << cube.inputs() << " => ";
    for (outputs_value_type nth_bit{}; nth_bit != output_bit_count; ++nth_bit) {
        out.put(cube.outputs() & (1u << nth_bit) ? '1' : '0');
    }
    return out;
}


#endif // EGT_COMPARISON_CUBE_HPP
