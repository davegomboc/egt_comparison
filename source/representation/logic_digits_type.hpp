#if !defined(EGT_COMPARISON_REPRESENTATION_LOGIC_DIGITS_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_LOGIC_DIGITS_TYPE_HPP


#include <cstdint>
#include <initializer_list>
#include <iosfwd>
#include <string>
#include <vector>

#include "bit_indexing_and_sizing.hpp"

#include "logic_digit_enum.hpp"

#include "representation/logic_digit_type.hpp"


namespace representation {


class logic_digits_type {
public:
    logic_digits_type & operator=(logic_digits_type const & other) = default;
    logic_digits_type & operator=(logic_digits_type && other) = default;
    logic_digits_type(logic_digits_type const & other) = default;
    logic_digits_type(logic_digits_type && other) = default;
    ~logic_digits_type() = default;

    logic_digits_type() {}
    logic_digits_type(std::initializer_list<logic_digit_type> init)
     : the_logic_digits(init) {}

    template <typename input_iterator_type>
    logic_digits_type(
        input_iterator_type begin,
        input_iterator_type end
    ) {
        std::copy(begin, end, the_logic_digits.cend());
    }

    template <typename uint_type>
    logic_digits_type(std::vector<uint_type> const & values);

    std::string to_string() const;

    template <typename uint_type>
    std::vector<uint_type> to_vector(std::size_t trailing_bit_count = 0) const;

    void reserve(std::size_t desired_minimum_capacity) {
        the_logic_digits.reserve(desired_minimum_capacity);
    }

    void push_back(logic_digit_type const & logic_digit) {
        the_logic_digits.push_back(logic_digit);
    }
    void push_back(logic_digit_enum const & logic_digit) {
        push_back(logic_digit_type(logic_digit));
    }

    auto cbegin() const { return the_logic_digits.cbegin(); }
    auto cend() const { return the_logic_digits.cend(); }
    auto begin() const { return the_logic_digits.begin(); }
    auto end() const { return the_logic_digits.end(); }
    auto begin() { return the_logic_digits.begin(); }
    auto end() { return the_logic_digits.end(); }

    bool empty() const { return the_logic_digits.empty(); }
    std::size_t size() const { return the_logic_digits.size(); }

    logic_digit_type const & operator[](std::size_t index) const {
        assert(index < the_logic_digits.size());
        return the_logic_digits[index];
    }
    logic_digit_type & operator[](std::size_t index) {
        assert(index < the_logic_digits.size());
        return the_logic_digits[index];
    }

    void append(logic_digits_type const & other) {
        the_logic_digits.insert(the_logic_digits.cend(),
         other.cbegin(), other.cend());
    }

private:
    std::vector<logic_digit_type> the_logic_digits;
};


template <typename uint_type>
logic_digits_type::logic_digits_type(std::vector<uint_type> const & values) {
    std::size_t const required_logic_digit_count{
     values.size() * (bits_per<uint_type> / 2)};
    the_logic_digits.resize(required_logic_digit_count);
    std::size_t nth_logic_digit{};
    std::size_t nth_vector_element{};
    uint_type current_value{};
    while (required_logic_digit_count != nth_logic_digit) {
        if (0 == which_bit_of<uint_type>(2 * nth_logic_digit)) {
            current_value = values[nth_vector_element++];
        }
        the_logic_digits[nth_logic_digit++] =
         logic_digit_type(logic_digit_enum(current_value & 0b11));
        current_value >>= 2;
    }
}

template <typename uint_type>
std::vector<uint_type> logic_digits_type::to_vector(
    std::size_t const trailing_bit_count
) const {
    std::size_t const bits_required{
     2 * the_logic_digits.size() + trailing_bit_count};
    std::vector<uint_type> result(required<uint_type>(bits_required));
    std::size_t nth_logic_digit{};
    std::size_t nth_vector_element{};
    unsigned int shift_count{};
    while (the_logic_digits.size() != nth_logic_digit) {
        uint_type const logic_digit_value{static_cast<uint_type>(
         the_logic_digits[nth_logic_digit].as_logic_digit_enum())};
        result[nth_vector_element] |=
         static_cast<uint_type>(logic_digit_value << shift_count);
        ++nth_logic_digit;
        shift_count += 2;
        if (shift_count == bits_per<uint_type>) {
            shift_count = 0;
            ++nth_vector_element;
        }
    }
    return result;
}

inline bool operator==(
    logic_digits_type const & lhs,
    logic_digits_type const & rhs
) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (std::size_t nth_logic_digit{}; nth_logic_digit != lhs.size();
     ++nth_logic_digit) {
        if (lhs[nth_logic_digit] != rhs[nth_logic_digit]) {
            return false;
        }
    }
    return true;
}

inline bool operator!=(
    logic_digits_type const & lhs,
    logic_digits_type const & rhs
) {
    return not operator==(lhs, rhs);
}


std::ostream & operator<<(std::ostream & out, logic_digits_type logic_digit);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_LOGIC_DIGITS_TYPE_HPP
