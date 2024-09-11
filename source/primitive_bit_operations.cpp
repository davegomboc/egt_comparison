#include "primitive_bit_operations.hpp"


#include <cstdint>


template bool has_single_bit<std::uint8_t>(std::uint8_t);
template bool has_single_bit<std::uint16_t>(std::uint16_t);
template bool has_single_bit<std::uint32_t>(std::uint32_t);
template bool has_single_bit<std::uint64_t>(std::uint64_t);

template std::uint8_t invert<std::uint8_t>(std::uint8_t);
template std::uint16_t invert<std::uint16_t>(std::uint16_t);
template std::uint32_t invert<std::uint32_t>(std::uint32_t);
template std::uint64_t invert<std::uint64_t>(std::uint64_t);

template std::uint8_t rotate_left<std::uint8_t>(std::uint8_t, unsigned int);
template std::uint16_t rotate_left<std::uint16_t>(std::uint16_t, unsigned int);
template std::uint32_t rotate_left<std::uint32_t>(std::uint32_t, unsigned int);
template std::uint64_t rotate_left<std::uint64_t>(std::uint64_t, unsigned int);

template std::uint8_t rotate_right<std::uint8_t>(std::uint8_t, unsigned int);
template std::uint16_t rotate_right<std::uint16_t>(std::uint16_t, unsigned int);
template std::uint32_t rotate_right<std::uint32_t>(std::uint32_t, unsigned int);
template std::uint64_t rotate_right<std::uint64_t>(std::uint64_t, unsigned int);

template std::int8_t const all_bits_set<std::int8_t>;
template std::int8_t const all_bits_set<std::int8_t const>;
template std::int16_t const all_bits_set<std::int16_t>;
template std::int16_t const all_bits_set<std::int16_t const>;
template std::int32_t const all_bits_set<std::int32_t>;
template std::int32_t const all_bits_set<std::int32_t const>;
template std::int64_t const all_bits_set<std::int64_t>;
template std::int64_t const all_bits_set<std::int64_t const>;
template std::uint8_t const all_bits_set<std::uint8_t>;
template std::uint8_t const all_bits_set<std::uint8_t const>;
template std::uint16_t const all_bits_set<std::uint16_t>;
template std::uint16_t const all_bits_set<std::uint16_t const>;
template std::uint32_t const all_bits_set<std::uint32_t>;
template std::uint32_t const all_bits_set<std::uint32_t const>;
template std::uint64_t const all_bits_set<std::uint64_t>;
template std::uint64_t const all_bits_set<std::uint64_t const>;

template std::int8_t const no_bits_set<std::int8_t>;
template std::int8_t const no_bits_set<std::int8_t const>;
template std::int16_t const no_bits_set<std::int16_t>;
template std::int16_t const no_bits_set<std::int16_t const>;
template std::int32_t const no_bits_set<std::int32_t>;
template std::int32_t const no_bits_set<std::int32_t const>;
template std::int64_t const no_bits_set<std::int64_t>;
template std::int64_t const no_bits_set<std::int64_t const>;
template std::uint8_t const no_bits_set<std::uint8_t>;
template std::uint8_t const no_bits_set<std::uint8_t const>;
template std::uint16_t const no_bits_set<std::uint16_t>;
template std::uint16_t const no_bits_set<std::uint16_t const>;
template std::uint32_t const no_bits_set<std::uint32_t>;
template std::uint32_t const no_bits_set<std::uint32_t const>;
template std::uint64_t const no_bits_set<std::uint64_t>;
template std::uint64_t const no_bits_set<std::uint64_t const>;

template std::int8_t const even_bits_set<std::int8_t>;
template std::int8_t const even_bits_set<std::int8_t const>;
template std::int16_t const even_bits_set<std::int16_t>;
template std::int16_t const even_bits_set<std::int16_t const>;
template std::int32_t const even_bits_set<std::int32_t>;
template std::int32_t const even_bits_set<std::int32_t const>;
template std::int64_t const even_bits_set<std::int64_t>;
template std::int64_t const even_bits_set<std::int64_t const>;
template std::uint8_t const even_bits_set<std::uint8_t>;
template std::uint8_t const even_bits_set<std::uint8_t const>;
template std::uint16_t const even_bits_set<std::uint16_t>;
template std::uint16_t const even_bits_set<std::uint16_t const>;
template std::uint32_t const even_bits_set<std::uint32_t>;
template std::uint32_t const even_bits_set<std::uint32_t const>;
template std::uint64_t const even_bits_set<std::uint64_t>;
template std::uint64_t const even_bits_set<std::uint64_t const>;

template std::int8_t const odd_bits_set<std::int8_t>;
template std::int8_t const odd_bits_set<std::int8_t const>;
template std::int16_t const odd_bits_set<std::int16_t>;
template std::int16_t const odd_bits_set<std::int16_t const>;
template std::int32_t const odd_bits_set<std::int32_t>;
template std::int32_t const odd_bits_set<std::int32_t const>;
template std::int64_t const odd_bits_set<std::int64_t>;
template std::int64_t const odd_bits_set<std::int64_t const>;
template std::uint8_t const odd_bits_set<std::uint8_t>;
template std::uint8_t const odd_bits_set<std::uint8_t const>;
template std::uint16_t const odd_bits_set<std::uint16_t>;
template std::uint16_t const odd_bits_set<std::uint16_t const>;
template std::uint32_t const odd_bits_set<std::uint32_t>;
template std::uint32_t const odd_bits_set<std::uint32_t const>;
template std::uint64_t const odd_bits_set<std::uint64_t>;
template std::uint64_t const odd_bits_set<std::uint64_t const>;
