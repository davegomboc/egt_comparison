#if !defined(EGT_COMPARISON_REPRESENTATION_CUBE_SCHEMA_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_CUBE_SCHEMA_TYPE_HPP


#include <cstddef>


namespace representation {


class cube_schema_type {
public:
    cube_schema_type & operator=(cube_schema_type const &) = default;
    cube_schema_type & operator=(cube_schema_type &&) = default;
    cube_schema_type(cube_schema_type const &) = default;
    cube_schema_type(cube_schema_type &&) = default;
    ~cube_schema_type() = default;
    cube_schema_type(
        std::size_t binary_variable_count,
        std::size_t output_count
    ) : the_binary_variable_count(binary_variable_count),
        the_output_count(output_count) {}

    std::size_t binary_variable_count() const {
        return the_binary_variable_count;
    }

    std::size_t binary_variable_bit_count() const {
        return 2 * binary_variable_count();
    }

    std::size_t output_count() const {
        return the_output_count;
    }

    std::size_t bit_count() const {
        return binary_variable_bit_count() + output_count();
    }

    std::size_t position_count() const {
        if (0 == binary_variable_count()) {
            return 0;
        }
        return 1ull << binary_variable_count();
    }

    std::size_t single_side_to_move_position_count() const {
        if (0 == binary_variable_count()) {
            return 0;
        }
        return 1ull << (binary_variable_count() - 1);
    }

private:
    std::size_t the_binary_variable_count;
    std::size_t the_output_count;
};


inline bool operator==(
    cube_schema_type const & lhs,
    cube_schema_type const & rhs
) {
    return (lhs.binary_variable_count() == rhs.binary_variable_count())
     and (lhs.output_count() == rhs.output_count());
}


inline bool operator!=(
    cube_schema_type const & lhs,
    cube_schema_type const & rhs
) {
    return not operator==(lhs, rhs);
}


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_CUBE_SCHEMA_TYPE_HPP
