#if !defined(EGT_COMPARISON_BIT_INDEXING_AND_SIZING_HPP)
#define EGT_COMPARISON_BIT_INDEXING_AND_SIZING_HPP


#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "primitive_bit_operations.hpp"


template <typename uint_type>
constexpr unsigned int bits_per{CHAR_BIT * sizeof(uint_type)};
static_assert(8 == bits_per<std::uint8_t>, "bits_per<std::uint8_t> was not 8.");
static_assert(16 == bits_per<std::uint16_t>,
 "bits_per<std::uint16_t> was not 16.");
static_assert(32 == bits_per<std::uint32_t>,
 "bits_per<std::uint32_t> was not 32.");
static_assert(64 == bits_per<std::uint64_t>,
 "bits_per<std::uint64_t> was not 64.");
static_assert((8 == bits_per<unsigned int>) or (16 == bits_per<unsigned int>)
 or (32 == bits_per<unsigned int>) or (64 == bits_per<unsigned int>),
 "bits_per<unsigned int> was neither 8, nor 16, nor 32, nor 64.");


template <typename uint_type>
constexpr unsigned int lg_bits_per{
 count_trailing_zeroes<uint_type>(bits_per<uint_type>)};
static_assert(3 == lg_bits_per<std::uint8_t>, "lg_bits_per_uint8_t was not 3.");
static_assert(4 == lg_bits_per<std::uint16_t>,
 "lg_bits_per_uint16_t was not 4.");
static_assert(5 == lg_bits_per<std::uint32_t>,
 "lg_bits_per_uint32_t was not 5.");
static_assert(6 == lg_bits_per<std::uint64_t>,
 "lg_bits_per_uint64_t was not 6.");


template <typename uint_type>
constexpr std::size_t required(std::size_t const desired_bit_count) {
    return (desired_bit_count + bits_per<uint_type> - 1)
     >> lg_bits_per<uint_type>;
}
static_assert(0 == required<std::uint8_t>(0),
 "0 bits can be completely represented within 0 uint8_ts.");
static_assert(1 == required<std::uint8_t>(1),
 "1 bit can be completely represented within 1 uint8_t.");
static_assert(1 == required<std::uint8_t>(8),
 "8 bits can be completely represented within 1 uint8_t.");
static_assert(2 == required<std::uint8_t>(9),
 "9 bits can be completely represented within 2 uint8_ts.");
static_assert(2 == required<std::uint8_t>(16),
 "16 bits can be completely represented within 2 uint8_ts.");
static_assert(3 == required<std::uint8_t>(17),
 "17 bits can be completely represented within 3 uint8_ts.");

static_assert(0 == required<std::uint16_t>(0),
 "0 bits can be completely represented within 0 uint16_ts.");
static_assert(1 == required<std::uint16_t>(1),
 "1 bit can be completely represented within 1 uint16_t.");
static_assert(1 == required<std::uint16_t>(16),
 "16 bits can be completely represented within 1 uint16_t.");
static_assert(2 == required<std::uint16_t>(17),
 "17 bits can be completely represented within 2 uint16_ts.");
static_assert(2 == required<std::uint16_t>(32),
 "32 bits can be completely represented within 2 uint16_ts.");
static_assert(3 == required<std::uint16_t>(33),
 "33 bits can be completely represented within 3 uint16_ts.");

static_assert(0 == required<std::uint32_t>(0),
 "0 bits can be completely represented within 0 uint32_ts.");
static_assert(1 == required<std::uint32_t>(1),
 "1 bit can be completely represented within 1 uint32_t.");
static_assert(1 == required<std::uint32_t>(32),
 "32 bits can be completely represented within 1 uint32_t.");
static_assert(2 == required<std::uint32_t>(33),
 "33 bits can be completely represented within 2 uint32_ts.");
static_assert(2 == required<std::uint32_t>(64),
 "64 bits can be completely represented within 2 uint32_ts.");
static_assert(3 == required<std::uint32_t>(65),
 "65 bits can be completely represented within 3 uint32_ts.");

static_assert(0 == required<std::uint64_t>(0),
 "0 bits can be completely represented within 0 uint64_ts.");
static_assert(1 == required<std::uint64_t>(1),
 "1 bit can be completely represented within 1 uint64_t.");
static_assert(1 == required<std::uint64_t>(64),
 "64 bits can be completely represented within 1 uint64_t.");
static_assert(2 == required<std::uint64_t>(65),
 "65 bits can be completely represented within 2 uint64_ts.");
static_assert(2 == required<std::uint64_t>(128),
 "128 bits can be completely represented within 2 uint64_ts.");
static_assert(3 == required<std::uint64_t>(129),
 "129 bits can be completely represented within 3 uint64_ts.");


template <typename uint_type>
constexpr std::size_t which(std::size_t const bit_n) {
    return bit_n >> lg_bits_per<uint_type>;
}
static_assert(0 == which<std::uint8_t>(0), "uint8_t bits[0] contains bit 0.");
static_assert(0 == which<std::uint8_t>(7), "uint8_t bits[0] contains bit 7.");
static_assert(1 == which<std::uint8_t>(8), "uint8_t bits[1] contains bit 8.");
static_assert(1 == which<std::uint8_t>(15), "uint8_t bits[1] contains bit 15.");
static_assert(2 == which<std::uint8_t>(16), "uint8_t bits[2] contains bit 16.");

static_assert(0 == which<std::uint16_t>(0), "uint16_t bits[0] contains bit 0.");
static_assert(0 == which<std::uint16_t>(15),
 "uint16_t bits[0] contains bit 15.");
static_assert(1 == which<std::uint16_t>(16),
 "uint16_t bits[1] contains bit 16.");
static_assert(1 == which<std::uint16_t>(31),
 "uint16_t bits[1] contains bit 31.");
static_assert(2 == which<std::uint16_t>(32),
 "uint16_t bits[2] contains bit 32.");

static_assert(0 == which<std::uint32_t>(0), "uint32_t bits[0] contains bit 0.");
static_assert(0 == which<std::uint32_t>(31),
 "uint32_t bits[0] contains bit 31.");
static_assert(1 == which<std::uint32_t>(32),
 "uint32_t bits[1] contains bit 32.");
static_assert(1 == which<std::uint32_t>(63),
 "uint32_t bits[1] contains bit 63.");
static_assert(2 == which<std::uint32_t>(64),
 "uint32_t bits[2] contains bit 64.");

static_assert(0 == which<std::uint64_t>(0), "uint64_t bits[0] contains bit 0.");
static_assert(0 == which<std::uint64_t>(63),
 "uint64_t bits[0] contains bit 63.");
static_assert(1 == which<std::uint64_t>(64),
 "uint64_t bits[1] contains bit 64.");
static_assert(1 == which<std::uint64_t>(127),
 "uint64_t bits[1] contains bit 127.");
static_assert(2 == which<std::uint64_t>(128),
 "uint64_t bits[2] contains bit 128.");


template <typename uint_type>
constexpr unsigned int which_bit_of(std::size_t const bit_n) {
    return bit_n & (bits_per<uint_type> - 1);
}
static_assert(0 == which_bit_of<std::uint8_t>(0),
 "bit 0 of uint8_t bits[] is bit 0 of some particular uint8_t.");
static_assert(7 == which_bit_of<std::uint8_t>(7),
 "bit 7 of uint8_t bits[] is bit 7 of some particular uint8_t.");
static_assert(0 == which_bit_of<std::uint8_t>(8),
 "bit 8 of uint8_t bits[] is bit 0 of some particular uint8_t.");
static_assert(7 == which_bit_of<std::uint8_t>(15),
 "bit 15 of uint8_t bits[] is bit 7 of some particular uint8_t.");
static_assert(0 == which_bit_of<std::uint8_t>(16),
 "bit 16 of uint8_t bits[] is bit 0 of some particular uint8_t.");

static_assert(0 == which_bit_of<std::uint16_t>(0),
 "bit 0 of uint16_t bits[] is bit 0 of some particular uint16_t.");
static_assert(15 == which_bit_of<std::uint16_t>(15),
 "bit 15 of uint16_t bits[] is bit 15 of some particular uint16_t.");
static_assert(0 == which_bit_of<std::uint16_t>(16),
 "bit 16 of uint16_t bits[] is bit 0 of some particular uint16_t.");
static_assert(15 == which_bit_of<std::uint16_t>(31),
 "bit 31 of uint16_t bits[] is bit 15 of some particular uint16_t.");
static_assert(0 == which_bit_of<std::uint16_t>(32),
 "bit 32 of uint16_t bits[] is bit 0 of some particular uint16_t.");

static_assert(0 == which_bit_of<std::uint32_t>(0),
 "bit 0 of uint32_t bits[] is bit 0 of some particular uint32_t.");
static_assert(31 == which_bit_of<std::uint32_t>(31),
 "bit 31 of uint32_t bits[] is bit 31 of some particular uint32_t.");
static_assert(0 == which_bit_of<std::uint32_t>(32),
 "bit 32 of uint32_t bits[] is bit 0 of some particular uint32_t.");
static_assert(31 == which_bit_of<std::uint32_t>(63),
 "bit 63 of uint32_t bits[] is bit 31 of some particular uint32_t.");
static_assert(0 == which_bit_of<std::uint32_t>(64),
 "bit 64 of uint32_t bits[] is bit 0 of some particular uint32_t.");

static_assert(0 == which_bit_of<std::uint64_t>(0),
 "bit 0 of uint64_t bits[] is bit 0 of some particular uint64_t.");
static_assert(63 == which_bit_of<std::uint64_t>(63),
 "bit 63 of uint64_t bits[] is bit 31 of some particular uint64_t.");
static_assert(0 == which_bit_of<std::uint64_t>(64),
 "bit 64 of uint64_t bits[] is bit 0 of some particular uint64_t.");
static_assert(63 == which_bit_of<std::uint64_t>(127),
 "bit 127 of uint64_t bits[] is bit 31 of some particular uint64_t.");
static_assert(0 == which_bit_of<std::uint64_t>(128),
 "bit 128 of uint64_t bits[] is bit 0 of some particular uint64_t.");


template <typename uint_type>
constexpr uint_type one_hot_of(std::size_t const bit_n) {
    return static_cast<uint_type>(1ull << which_bit_of<uint_type>(bit_n));
}
static_assert(UINT8_C(0b00000001) == one_hot_of<std::uint8_t>(0),
 "bit 0 of uint8_t bits[] uses bitmask 0b00000001 for isolation.");
static_assert(UINT8_C(0b10000000)== one_hot_of<std::uint8_t>(7),
 "bit 7 of uint8_t bits[] uses bitmask 0b10000000 for isolation.");
static_assert(UINT8_C(0b00000001) == one_hot_of<std::uint8_t>(8),
 "bit 8 of uint8_t bits[] uses bitmask 0b00000001 for isolation.");
static_assert(UINT8_C(0b10000000) == one_hot_of<std::uint8_t>(15),
 "bit 15 of uint8_t bits[] uses bitmask 0b10000000 for isolation.");
static_assert(UINT8_C(0b00000001) == one_hot_of<std::uint8_t>(16),
 "bit 16 of uint8_t bits[] uses bitmask 0b00000001 for isolation.");

static_assert(UINT16_C(0b0000000000000001) == one_hot_of<std::uint16_t>(0),
 "bit 0 of uint16_t bits[] uses bitmask 0b0000000000000001 for isolation.");
static_assert(UINT16_C(0b1000000000000000) == one_hot_of<std::uint16_t>(15),
 "bit 15 of uint16_t bits[] uses bitmask 0b1000000000000000 for isolation.");
static_assert(UINT16_C(0b0000000000000001) == one_hot_of<std::uint16_t>(16),
 "bit 16 of uint16_t bits[] uses bitmask 0b0000000000000001 for isolation.");
static_assert(UINT16_C(0b1000000000000000) == one_hot_of<std::uint16_t>(31),
 "bit 31 of uint16_t bits[] uses bitmask 0b1000000000000000 for isolation.");
static_assert(UINT16_C(0b0000000000000001) == one_hot_of<std::uint16_t>(32),
 "bit 32 of uint16_t bits[] uses bitmask 0b0000000000000001 for isolation.");

static_assert(UINT32_C(0x00000001) == one_hot_of<std::uint32_t>(0),
 "bit 0 of uint32_t bits[] uses bitmask 0x00000001 for isolation.");
static_assert(UINT32_C(0x80000000) == one_hot_of<std::uint32_t>(31),
 "bit 31 of uint32_t bits[] uses bitmask 0x80000000 for isolation.");
static_assert(UINT32_C(0x00000001) == one_hot_of<std::uint32_t>(32),
 "bit 32 of uint32_t bits[] uses bitmask 0x00000001 for isolation.");
static_assert(UINT32_C(0x80000000) == one_hot_of<std::uint32_t>(63),
 "bit 63 of uint32_t bits[] uses bitmask 0x80000000 for isolation.");
static_assert(UINT32_C(0x00000001) == one_hot_of<std::uint32_t>(64),
 "bit 64 of uint32_t bits[] uses bitmask 0x00000001 for isolation.");

static_assert(UINT64_C(0x0000000000000001) == one_hot_of<std::uint64_t>(0),
 "bit 0 of uint64_t bits[] uses bitmask 0x0000000000000001 for isolation.");
static_assert(UINT64_C(0x8000000000000000) == one_hot_of<std::uint64_t>(63),
 "bit 63 of uint64_t bits[] uses bitmask 0x8000000000000000 for isolation.");
static_assert(UINT64_C(0x0000000000000001) == one_hot_of<std::uint64_t>(64),
 "bit 64 of uint64_t bits[] uses bitmask 0x0000000000000001 for isolation.");
static_assert(UINT64_C(0x8000000000000000) == one_hot_of<std::uint64_t>(127),
 "bit 127 of uint64_t bits[] uses bitmask 0x8000000000000000 for isolation.");
static_assert(UINT64_C(0x0000000000000001) == one_hot_of<std::uint64_t>(128),
 "bit 128 of uint64_t bits[] uses bitmask 0x0000000000000001 for isolation.");


template <typename uint_type>
constexpr uint_type n_hot_of(
    std::size_t const n,
    unsigned int const bit_places_to_rotate_left
) {
    return bits_per<uint_type> <= n ? invert<uint_type>(0)
     : rotate_left<uint_type>(static_cast<uint_type>(
     (1ull << which_bit_of<uint_type>(n)) - 1), bit_places_to_rotate_left);
}
static_assert(UINT8_C(0b00000000) == n_hot_of<std::uint8_t>(0, 0));
static_assert(UINT8_C(0b00000000) == n_hot_of<std::uint8_t>(0, 8));
static_assert(UINT8_C(0b00000001) == n_hot_of<std::uint8_t>(1, 0));
static_assert(UINT8_C(0b10000000) == n_hot_of<std::uint8_t>(1, 7));
static_assert(UINT8_C(0b00000011) == n_hot_of<std::uint8_t>(2, 0));
static_assert(UINT8_C(0b11000000) == n_hot_of<std::uint8_t>(2, 6));
static_assert(UINT8_C(0b00000111) == n_hot_of<std::uint8_t>(3, 0));
static_assert(UINT8_C(0b11100000) == n_hot_of<std::uint8_t>(3, 5));
static_assert(UINT8_C(0b00001111) == n_hot_of<std::uint8_t>(4, 0));
static_assert(UINT8_C(0b11110000) == n_hot_of<std::uint8_t>(4, 4));
static_assert(UINT8_C(0b00011111) == n_hot_of<std::uint8_t>(5, 0));
static_assert(UINT8_C(0b11111000) == n_hot_of<std::uint8_t>(5, 3));
static_assert(UINT8_C(0b00111111) == n_hot_of<std::uint8_t>(6, 0));
static_assert(UINT8_C(0b11111100) == n_hot_of<std::uint8_t>(6, 2));
static_assert(UINT8_C(0b01111111) == n_hot_of<std::uint8_t>(7, 0));
static_assert(UINT8_C(0b11111110) == n_hot_of<std::uint8_t>(7, 1));
static_assert(UINT8_C(0b11111111) == n_hot_of<std::uint8_t>(8, 0));
static_assert(UINT8_C(0b11111111) == n_hot_of<std::uint8_t>(8, 8));

static_assert(UINT16_C(0x0000) == n_hot_of<std::uint16_t>(0, 0));
static_assert(UINT16_C(0x0000) == n_hot_of<std::uint16_t>(0, 16));
static_assert(UINT16_C(0x0001) == n_hot_of<std::uint16_t>(1, 0));
static_assert(UINT16_C(0x8000) == n_hot_of<std::uint16_t>(1, 15));
static_assert(UINT16_C(0x7fff) == n_hot_of<std::uint16_t>(15, 0));
static_assert(UINT16_C(0xfffe) == n_hot_of<std::uint16_t>(15, 1));
static_assert(UINT16_C(0xffff) == n_hot_of<std::uint16_t>(16, 0));
static_assert(UINT16_C(0xffff) == n_hot_of<std::uint16_t>(16, 16));

static_assert(UINT32_C(0x00000000) == n_hot_of<std::uint32_t>(0, 0));
static_assert(UINT32_C(0x00000000) == n_hot_of<std::uint32_t>(0, 32));
static_assert(UINT32_C(0x00000001) == n_hot_of<std::uint32_t>(1, 0));
static_assert(UINT32_C(0x80000000) == n_hot_of<std::uint32_t>(1, 31));
static_assert(UINT32_C(0x7fffffff) == n_hot_of<std::uint32_t>(31, 0));
static_assert(UINT32_C(0xfffffffe) == n_hot_of<std::uint32_t>(31, 1));
static_assert(UINT32_C(0xffffffff) == n_hot_of<std::uint32_t>(32, 0));
static_assert(UINT32_C(0xffffffff) == n_hot_of<std::uint32_t>(32, 32));

static_assert(UINT64_C(0x0000000000000000) == n_hot_of<std::uint64_t>(0, 0));
static_assert(UINT64_C(0x0000000000000000) == n_hot_of<std::uint64_t>(0, 64));
static_assert(UINT64_C(0x0000000000000001) == n_hot_of<std::uint64_t>(1, 0));
static_assert(UINT64_C(0x8000000000000000) == n_hot_of<std::uint64_t>(1, 63));
static_assert(UINT64_C(0x7fffffffffffffff) == n_hot_of<std::uint64_t>(63, 0));
static_assert(UINT64_C(0xfffffffffffffffe) == n_hot_of<std::uint64_t>(63, 1));
static_assert(UINT64_C(0xffffffffffffffff) == n_hot_of<std::uint64_t>(64, 0));
static_assert(UINT64_C(0xffffffffffffffff) == n_hot_of<std::uint64_t>(64, 64));


template <typename uint_type>
constexpr unsigned int used_bit_count_in_final_place(
    std::size_t const desired_bit_count
) {
    unsigned int const remainder{static_cast<unsigned int>(
     desired_bit_count % bits_per<uint_type>)};
    unsigned int const result{remainder ? remainder : bits_per<uint_type>};
    return result;
}
static_assert(1 == used_bit_count_in_final_place<std::uint8_t>(1),
 "1 bit via uint8_t[] uses 1 bit in the final place.");
static_assert(8 == used_bit_count_in_final_place<std::uint8_t>(8),
 "8 bits via uint8_t[] uses 8 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint8_t>(9),
 "9 bits via uint8_t[] uses 1 bit in the final place.");
static_assert(8 == used_bit_count_in_final_place<std::uint8_t>(16),
 "16 bits via uint8_t[] uses 8 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint8_t>(17),
 "17 bits via uint8_t[] uses 1 bit in the final place.");

static_assert(1 == used_bit_count_in_final_place<std::uint16_t>(1),
 "1 bit via uint16_t[] uses 1 bit in the final place.");
static_assert(16 == used_bit_count_in_final_place<std::uint16_t>(16),
 "16 bits via uint16_t[] uses 16 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint16_t>(17),
 "17 bits via uint16_t[] uses 1 bit in the final place.");
static_assert(16 == used_bit_count_in_final_place<std::uint16_t>(32),
 "32 bits via uint16_t[] uses 16 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint16_t>(33),
 "33 bits via uint16_t[] uses 1 bit in the final place.");

static_assert(1 == used_bit_count_in_final_place<std::uint32_t>(1),
 "1 bit via uint32_t[] uses 1 bit in the final place.");
static_assert(32 == used_bit_count_in_final_place<std::uint32_t>(32),
 "32 bits via uint32_t[] uses 32 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint32_t>(33),
 "33 bits via uint32_t[] uses 1 bit in the final place.");
static_assert(32 == used_bit_count_in_final_place<std::uint32_t>(64),
 "64 bits via uint32_t[] uses 32 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint32_t>(65),
 "65 bits via uint32_t[] uses 1 bit in the final place.");

static_assert(1 == used_bit_count_in_final_place<std::uint64_t>(1),
 "1 bit via uint64_t[] uses 1 bit in the final place.");
static_assert(64 == used_bit_count_in_final_place<std::uint64_t>(64),
 "64 bits via uint64_t[] uses 64 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint64_t>(65),
 "65 bits via uint64_t[] uses 1 bit in the final place.");
static_assert(64 == used_bit_count_in_final_place<std::uint64_t>(128),
 "128 bits via uint64_t[] uses 64 bits in the final place.");
static_assert(1 == used_bit_count_in_final_place<std::uint64_t>(129),
 "129 bits via uint64_t[] uses 1 bit in the final place.");


template <typename uint_type>
constexpr uint_type used_bit_mask_for_final_place(
    std::size_t const desired_bit_count
) {
    unsigned int const final_place_used_bit_count{
     used_bit_count_in_final_place<uint_type>(desired_bit_count)};
    uint_type const result{bits_per<uint_type> == final_place_used_bit_count
     ? std::numeric_limits<uint_type>::max() : static_cast<uint_type>(
     one_hot_of<uint_type>(final_place_used_bit_count) - 1)};
    return result;
}
static_assert(0b00000001 == used_bit_mask_for_final_place<std::uint8_t>(1),
 "1 bit via uint8_t[] uses mask 0b00000001 for the final place.");
static_assert(0b01111111 == used_bit_mask_for_final_place<std::uint8_t>(7),
 "7 bits via uint8_t[] uses mask 0b01111111 for the final place.");
static_assert(0b11111111 == used_bit_mask_for_final_place<std::uint8_t>(8),
 "8 bits via uint8_t[] uses mask 0b11111111 for the final place.");
static_assert(0b00000001 == used_bit_mask_for_final_place<std::uint8_t>(9),
 "9 bits via uint8_t[] uses mask 0b00000001 for the final place.");
static_assert(0b01111111 == used_bit_mask_for_final_place<std::uint8_t>(15),
 "15 bits via uint8_t[] uses mask 0b01111111 for the final place.");
static_assert(0b11111111 == used_bit_mask_for_final_place<std::uint8_t>(16),
 "16 bits via uint8_t[] uses mask 0b11111111 for the final place.");
static_assert(0b00000001 == used_bit_mask_for_final_place<std::uint8_t>(17),
 "17 bits via uint8_t[] uses mask 0b00000001 for the final place.");

static_assert(0b0000000000000001
 == used_bit_mask_for_final_place<std::uint16_t>(1),
 "1 bit via uint16_t[] uses mask 0b0000000000000001 for the final place.");
static_assert(0b0111111111111111
 == used_bit_mask_for_final_place<std::uint16_t>(15),
 "15 bits via uint16_t[] uses mask 0b0111111111111111 for the final place.");
static_assert(0b1111111111111111
 == used_bit_mask_for_final_place<std::uint16_t>(16),
 "16 bits via uint16_t[] uses mask 0b1111111111111111 for the final place.");
static_assert(0b0000000000000001
 == used_bit_mask_for_final_place<std::uint16_t>(17),
 "17 bits via uint16_t[] uses mask 0b0000000000000001 for the final place.");
static_assert(0b0111111111111111
 == used_bit_mask_for_final_place<std::uint16_t>(31),
 "31 bits via uint16_t[] uses mask 0b0111111111111111 for the final place.");
static_assert(0b1111111111111111
 == used_bit_mask_for_final_place<std::uint16_t>(32),
 "32 bits via uint16_t[] uses mask 0b1111111111111111 for the final place.");
static_assert(0b0000000000000001
 == used_bit_mask_for_final_place<std::uint16_t>(33),
 "33 bits via uint16_t[] uses mask 0b0000000000000001 for the final place.");

static_assert(0x00000001 == used_bit_mask_for_final_place<std::uint32_t>(1),
 "1 bit via uint32_t[] uses mask 0x00000001 for the final place.");
static_assert(0x7fffffff == used_bit_mask_for_final_place<std::uint32_t>(31),
 "31 bits via uint32_t[] uses mask 0x7fffffff for the final place.");
static_assert(0xffffffff == used_bit_mask_for_final_place<std::uint32_t>(32),
 "32 bits via uint32_t[] uses mask 0xffffffff for the final place.");
static_assert(0x00000001 == used_bit_mask_for_final_place<std::uint32_t>(33),
 "33 bits via uint32_t[] uses mask 0x00000001 for the final place.");
static_assert(0x7fffffff == used_bit_mask_for_final_place<std::uint32_t>(63),
 "63 bits via uint32_t[] uses mask 0x7fffffff for the final place.");
static_assert(0xffffffff == used_bit_mask_for_final_place<std::uint32_t>(64),
 "64 bits via uint32_t[] uses mask 0xffffffff for the final place.");
static_assert(0x00000001 == used_bit_mask_for_final_place<std::uint32_t>(65),
 "65 bits via uint32_t[] uses mask 0x00000001 for the final place.");

static_assert(0x0000000000000001 ==
 used_bit_mask_for_final_place<std::uint64_t>(1),
 "1 bit via uint64_t[] uses mask 0x0000000000000001 for the final place.");
static_assert(0x7fffffffffffffff ==
 used_bit_mask_for_final_place<std::uint64_t>(63),
 "63 bits via uint64_t[] uses mask 0x7fffffffffffffff for the final place.");
static_assert(0xffffffffffffffff ==
 used_bit_mask_for_final_place<std::uint64_t>(64),
 "64 bits via uint64_t[] uses mask 0xffffffffffffffff for the final place.");
static_assert(0x0000000000000001 ==
 used_bit_mask_for_final_place<std::uint64_t>(65),
 "65 bits via uint64_t[] uses mask 0x0000000000000001 for the final place.");
static_assert(0x7fffffffffffffff ==
 used_bit_mask_for_final_place<std::uint64_t>(127),
 "127 bits via uint64_t[] uses mask 0x7fffffffffffffff for the final place.");
static_assert(0xffffffffffffffff ==
 used_bit_mask_for_final_place<std::uint64_t>(128),
 "128 bits via uint64_t[] uses mask 0xffffffffffffffff for the final place.");
static_assert(0x0000000000000001 ==
 used_bit_mask_for_final_place<std::uint64_t>(129),
 "129 bits via uint64_t[] uses mask 0x0000000000000001 for the final place.");


template <typename uint_type>
constexpr uint_type single_uint_mask(
    std::size_t const first_bit,
    std::size_t const past_last_bit
) {
    assert(first_bit <= past_last_bit);
    assert(which<uint_type>(first_bit) == which<uint_type>(past_last_bit));
    std::size_t const run_length{past_last_bit - first_bit};
    unsigned int const rotate_left_count{which_bit_of<uint_type>(first_bit)};
    return n_hot_of<uint_type>(run_length, rotate_left_count);
}


template <typename uint_type>
constexpr uint_type first_uint_mask(std::size_t const first_bit) {
    unsigned int const index_of_first_bit_in_uint{
     which_bit_of<uint_type>(first_bit)};
    std::size_t const run_length{
     bits_per<uint_type> - index_of_first_bit_in_uint};
    unsigned int const rotate_left_count{index_of_first_bit_in_uint};
    return n_hot_of<uint_type>(run_length, rotate_left_count);
}
static_assert(0b11111111 == first_uint_mask<std::uint8_t>(0),
 "uint8_t[] with bits >= 0 set uses mask 0b11111111 for the first place.");
static_assert(0b11111110 == first_uint_mask<std::uint8_t>(1),
 "uint8_t[] with bits >= 1 set uses mask 0b11111110 for the first place.");
static_assert(0b10000000 == first_uint_mask<std::uint8_t>(7),
 "uint8_t[] with bits >= 7 set uses mask 0b10000000 for the first place.");
static_assert(0b11111111 == first_uint_mask<std::uint8_t>(8),
 "uint8_t[] with bits >= 8 set uses mask 0b11111111 for the first place.");
static_assert(0b11111110 == first_uint_mask<std::uint8_t>(9),
 "uint8_t[] with bits >= 9 set uses mask 0b11111110 for the first place.");
static_assert(0b10000000 == first_uint_mask<std::uint8_t>(15),
 "uint8_t[] with bits >= 15 set uses mask 0b10000000 for the first place.");
static_assert(0b11111111 == first_uint_mask<std::uint8_t>(16),
 "uint8_t[] with bits >= 16 set uses mask 0b11111111 for the first place.");
static_assert(0b11111110 == first_uint_mask<std::uint8_t>(17),
 "uint8_t[] with bits >= 17 set uses mask 0b11111110 for the first place.");

static_assert(0xffff == first_uint_mask<std::uint16_t>(0),
 "uint16_t[] with bits >= 0 set uses mask 0xffff for the first place.");
static_assert(0xfffe == first_uint_mask<std::uint16_t>(1),
 "uint16_t[] with bits >= 1 set uses mask 0xfffe for the first place.");
static_assert(0x8000 == first_uint_mask<std::uint16_t>(15),
 "uint16_t[] with bits >= 15 set uses mask 0x8000 for the first place.");
static_assert(0xffff == first_uint_mask<std::uint16_t>(16),
 "uint16_t[] with bits >= 16 set uses mask 0xffff for the first place.");
static_assert(0xfffe == first_uint_mask<std::uint16_t>(17),
 "uint16_t[] with bits >= 17 set uses mask 0xfffe for the first place.");
static_assert(0x8000 == first_uint_mask<std::uint16_t>(31),
 "uint16_t[] with bits >= 31 set uses mask 0x8000 for the first place.");
static_assert(0xffff == first_uint_mask<std::uint16_t>(32),
 "uint16_t[] with bits >= 32 set uses mask 0xffff for the first place.");
static_assert(0xfffe == first_uint_mask<std::uint16_t>(33),
 "uint16_t[] with bits >= 33 set uses mask 0xfffe for the first place.");

static_assert(0xffffffff == first_uint_mask<std::uint32_t>(0),
 "uint32_t[] with bits >= 0 set uses mask 0xffffffff for the first place.");
static_assert(0xfffffffe == first_uint_mask<std::uint32_t>(1),
 "uint32_t[] with bits >= 1 set uses mask 0xfffffffe for the first place.");
static_assert(0x80000000 == first_uint_mask<std::uint32_t>(31),
 "uint32_t[] with bits >= 31 set uses mask 0x80000000 for the first place.");
static_assert(0xffffffff == first_uint_mask<std::uint32_t>(32),
 "uint32_t[] with bits >= 32 set uses mask 0xffffffff for the first place.");
static_assert(0xfffffffe == first_uint_mask<std::uint32_t>(33),
 "uint32_t[] with bits >= 33 set uses mask 0xfffffffe for the first place.");
static_assert(0x80000000 == first_uint_mask<std::uint32_t>(63),
 "uint32_t[] with bits >= 63 set uses mask 0x80000000 for the first place.");
static_assert(0xffffffff == first_uint_mask<std::uint32_t>(64),
 "uint32_t[] with bits >= 64 set uses mask 0xffffffff for the first place.");
static_assert(0xfffffffe == first_uint_mask<std::uint32_t>(65),
 "uint32_t[] with bits >= 65 set uses mask 0xfffffffe for the first place.");

static_assert(0xffffffffffffffff == first_uint_mask<std::uint64_t>(0),
 "uint64_t[] with bits >= 0 set uses mask 0xffffffffffffffff for the first place.");
static_assert(0xfffffffffffffffe == first_uint_mask<std::uint64_t>(1),
 "uint64_t[] with bits >= 1 set uses mask 0xfffffffffffffffe for the first place.");
static_assert(0x8000000000000000 == first_uint_mask<std::uint64_t>(63),
 "uint64_t[] with bits >= 63 set uses mask 0x8000000000000000 for the first place.");
static_assert(0xffffffffffffffff == first_uint_mask<std::uint64_t>(64),
 "uint64_t[] with bits >= 64 set uses mask 0xffffffffffffffff for the first place.");
static_assert(0xfffffffffffffffe == first_uint_mask<std::uint64_t>(65),
 "uint64_t[] with bits >= 65 set uses mask 0xfffffffffffffffe for the first place.");
static_assert(0x8000000000000000 == first_uint_mask<std::uint64_t>(127),
 "uint64_t[] with bits >= 127 set uses mask 0x8000000000000000 for the first place.");
static_assert(0xffffffffffffffff == first_uint_mask<std::uint64_t>(128),
 "uint64_t[] with bits >= 128 set uses mask 0xffffffffffffffff for the first place.");
static_assert(0xfffffffffffffffe == first_uint_mask<std::uint64_t>(129),
 "uint64_t[] with bits >= 129 set uses mask 0xfffffffffffffffe for the first place.");

// Unlike used_bit_mask_for_final_place, this routine returns a zero mask (for
// the following uint_type, were it to exist) when the past_last_bit specified
// is a precise multiple of the bits available within a uint_type.
template <typename uint_type>
constexpr uint_type past_last_uint_mask(std::size_t const past_last_bit) {
    std::size_t const run_length{which_bit_of<uint_type>(past_last_bit)};
    unsigned int const rotate_left_count{};
    return n_hot_of<uint_type>(run_length, rotate_left_count);
}
static_assert(0b00000000 == past_last_uint_mask<std::uint8_t>(0),
 "0 bits via uint8_t[] points past all of the useful bits.");
static_assert(0b00000001 == past_last_uint_mask<std::uint8_t>(1),
 "1 bit via uint8_t[] uses mask 0b00000001 for the final place.");
static_assert(0b01111111 == past_last_uint_mask<std::uint8_t>(7),
 "7 bits via uint8_t[] uses mask 0b01111111 for the final place.");
static_assert(0b00000000 == past_last_uint_mask<std::uint8_t>(8),
 "8 bits via uint8_t[] points past all of the useful bits.");
static_assert(0b00000001 == past_last_uint_mask<std::uint8_t>(9),
 "9 bits via uint8_t[] uses mask 0b00000001 for the final place.");
static_assert(0b01111111 == past_last_uint_mask<std::uint8_t>(15),
 "15 bits via uint8_t[] uses mask 0b01111111 for the final place.");
static_assert(0b00000000 == past_last_uint_mask<std::uint8_t>(16),
 "16 bits via uint8_t[] points past all of the useful bits.");
static_assert(0b00000001 == past_last_uint_mask<std::uint8_t>(17),
 "17 bits via uint8_t[] uses mask 0b00000001 for the final place.");

static_assert(0b0000000000000000 == past_last_uint_mask<std::uint16_t>(0),
 "0 bits via uint16_t[] points past all of the useful bits.");
static_assert(0b0000000000000001 == past_last_uint_mask<std::uint16_t>(1),
 "1 bit via uint16_t[] uses mask 0b0000000000000001 for the final place.");
static_assert(0b0111111111111111 == past_last_uint_mask<std::uint16_t>(15),
 "15 bits via uint16_t[] uses mask 0b0111111111111111 for the final place.");
static_assert(0b0000000000000000 == past_last_uint_mask<std::uint16_t>(16),
 "16 bits via uint16_t[] points past all of the useful bits.");
static_assert(0b0000000000000001 == past_last_uint_mask<std::uint16_t>(17),
 "17 bits via uint16_t[] uses mask 0b0000000000000001 for the final place.");
static_assert(0b0111111111111111 == past_last_uint_mask<std::uint16_t>(31),
 "31 bits via uint16_t[] uses mask 0b0111111111111111 for the final place.");
static_assert(0b0000000000000000 == past_last_uint_mask<std::uint16_t>(32),
 "32 bits via uint16_t[] points past all of the useful bits.");
static_assert(0b0000000000000001 == past_last_uint_mask<std::uint16_t>(33),
 "33 bits via uint16_t[] uses mask 0b0000000000000001 for the final place.");

static_assert(0x00000000 == past_last_uint_mask<std::uint32_t>(0),
 "0 bits via uint32_t[] points past all of the useful bits.");
static_assert(0x00000001 == past_last_uint_mask<std::uint32_t>(1),
 "1 bit via uint32_t[] uses mask 0x00000001 for the final place.");
static_assert(0x7fffffff == past_last_uint_mask<std::uint32_t>(31),
 "31 bits via uint32_t[] uses mask 0x7fffffff for the final place.");
static_assert(0b00000000 == past_last_uint_mask<std::uint32_t>(32),
 "32 bits via uint32_t[] points past all of the useful bits.");
static_assert(0x00000001 == past_last_uint_mask<std::uint32_t>(33),
 "33 bits via uint32_t[] uses mask 0x00000001 for the final place.");
static_assert(0x7fffffff == past_last_uint_mask<std::uint32_t>(63),
 "63 bits via uint32_t[] uses mask 0x7fffffff for the final place.");
static_assert(0x00000000 == past_last_uint_mask<std::uint32_t>(64),
 "64 bits via uint32_t[] points past all of the useful bits.");
static_assert(0x00000001 == past_last_uint_mask<std::uint32_t>(65),
 "65 bits via uint32_t[] uses mask 0x00000001 for the final place.");

static_assert(0x0000000000000000 == past_last_uint_mask<std::uint64_t>(0),
 "0 bits via uint64_t[] points past all of the useful bits.");
static_assert(0x0000000000000001 == past_last_uint_mask<std::uint64_t>(1),
 "1 bit via uint64_t[] uses mask 0x0000000000000001 for the final place.");
static_assert(0x7fffffffffffffff == past_last_uint_mask<std::uint64_t>(63),
 "63 bits via uint64_t[] uses mask 0x7fffffffffffffff for the final place.");
static_assert(0b00000000 == past_last_uint_mask<std::uint64_t>(64),
 "64 bits via uint64_t[] points past all of the useful bits.");
static_assert(0x0000000000000001 == past_last_uint_mask<std::uint64_t>(65),
 "65 bits via uint64_t[] uses mask 0x0000000000000001 for the final place.");
static_assert(0x7fffffffffffffff == past_last_uint_mask<std::uint64_t>(127),
 "127 bits via uint64_t[] uses mask 0x7fffffffffffffff for the final place.");
static_assert(0x0000000000000000 == past_last_uint_mask<std::uint64_t>(128),
 "128 bits via uint64_t[] points past all of the useful bits.");
static_assert(0x0000000000000001 == past_last_uint_mask<std::uint64_t>(129),
 "129 bits via uint64_t[] uses mask 0x0000000000000001 for the final place.");


template <typename uint_type>
constexpr uint_type extract_single_uint_bit_run(
    uint_type const value,
    std::size_t const first_bit_of,
    std::size_t const past_last_bit_of
) {
    if ((0 == first_bit_of) and (bits_per<uint_type> == past_last_bit_of)) {
        return value;
    }
    assert(first_bit_of <= past_last_bit_of);
    assert(first_bit_of < bits_per<uint_type>);
    assert(past_last_bit_of <= bits_per<uint_type>);
    return static_cast<uint_type>((value >> first_bit_of)
     & ((static_cast<uint_type>(1) << (past_last_bit_of - first_bit_of)) - 1));
}
static_assert(UINT8_C(0b00000000) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b00001110), 0, 0));
static_assert(UINT8_C(0b00000110) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b00001110), 0, 3));
static_assert(UINT8_C(0b10101010) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b10101010), 0, 8));
static_assert(UINT8_C(0b00000000) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b11111111), 3, 3));
static_assert(UINT8_C(0b00000011) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b01110101), 5, 7));
static_assert(UINT8_C(0b00010110) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b10110101), 3, 8));
static_assert(UINT8_C(0b00000101) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b10100000), 5, 8));
static_assert(UINT8_C(0b00000000) == extract_single_uint_bit_run<std::uint8_t>(
 UINT8_C(0b01111111), 7, 8));


template <typename uint_type>
constexpr uint_type extract_bit_run_into_single_uint(
    uint_type const low_uint_value,
    uint_type const high_uint_value,
    std::size_t const first_bit_of,
    std::size_t const past_last_bit_of
) {
    assert(0 == which<uint_type>(first_bit_of));
    assert(past_last_bit_of - first_bit_of <= bits_per<uint_type>);
    if (past_last_bit_of <= bits_per<uint_type>) {
        return extract_single_uint_bit_run(low_uint_value, first_bit_of,
         past_last_bit_of);
    }
    uint_type needed_from_high_uint{static_cast<uint_type>(high_uint_value
     & used_bit_mask_for_final_place<uint_type>(past_last_bit_of))};
    needed_from_high_uint = rotate_right<uint_type>(needed_from_high_uint,
     static_cast<unsigned int>(first_bit_of));
    return needed_from_high_uint | extract_single_uint_bit_run(low_uint_value,
     first_bit_of, bits_per<uint_type>);
}
static_assert(UINT8_C(0b00010101) == extract_bit_run_into_single_uint<
 std::uint8_t>(UINT8_C(0b10100000), UINT8_C(0b00000010), 5, 10));


#endif // EGT_COMPARISON_BIT_INDEXING_AND_SIZING_HPP
