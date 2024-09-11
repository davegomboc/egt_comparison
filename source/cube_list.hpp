#ifndef EGT_COMPARISON_CUBE_LIST_TYPE_HPP
#define EGT_COMPARISON_CUBE_LIST_TYPE_HPP


#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <set>
#include <system_error>
#include <tuple>
#include <vector>

#include "cube.hpp"
#include "cube_binary_inputs.hpp"
#include "data_category_enum.hpp"
#include "filesystem_stuff.hpp"
#include "parallel_for_each.hpp"


template <
    std::size_t const MAXINBITS,
    std::size_t const MAXOUTBITS
> class cube_list_tt {
public:
    static std::size_t constexpr maxinbits{MAXINBITS};
    static std::size_t constexpr maxoutbits{MAXOUTBITS};

    using cube_type = cube_tt<MAXINBITS, MAXOUTBITS>;
    using inputs_value_type = typename cube_type::inputs_value_type;
    using inputs_index_type = typename cube_type::inputs_index_type;
    using outputs_value_type = typename cube_type::outputs_value_type;

    using iterator_type = typename std::vector<cube_type>::iterator;
    using const_iterator_type = typename std::vector<cube_type>::const_iterator;

private:
    // Hack to allow ddag to emplace_back upon the_cubes.
    template <std::size_t NDIM, std::size_t DTREEMAXOUTBITS, std::size_t INDEXBITS>
    friend class ddag;

    std::vector<cube_type> the_cubes;
    std::size_t the_ninbits;
    std::size_t the_noutbits;
    inputs_value_type the_lookup_mask;

    std::size_t mask1sort(
        inputs_value_type const mask,
        std::size_t low,
        std::size_t high
   ) {
        while (low < high) {
            if (mask & static_cast<inputs_value_type>((*this)[low].inputs())) {
                --high;
                while ((low < high) and (mask
                 & static_cast<inputs_value_type>((*this)[high].inputs()))) {
                    --high;
                }
                if (low != high) {
                    using std::swap;
                    swap((*this)[low], (*this)[high]);
                }
            } else {
                ++low;
            }
        }
        return low;
    }

public:
    // returns the tuple (i0, i1, i2)
    // such that
    // [low, i0) are where bit is 00
    // [i0, i1) is where bit is 01
    // [i1, i2) is where bit is 10
    // [i2, high) is where bit is 11
    //    (not a stable sort)
    std::tuple<std::size_t, std::size_t, std::size_t> sort1bit(
        std::size_t const nth_bit_pair,
        std::size_t const low,
        std::size_t const high
    ) {
        inputs_value_type const lsb_mask{
         static_cast<inputs_value_type>(1) << (2 * nth_bit_pair)};
        inputs_value_type const msb_mask{lsb_mask << 1};
        std::size_t const i1{mask1sort(msb_mask, low, high)};
        std::size_t const i0{mask1sort(lsb_mask, low, i1)};
        std::size_t const i2{mask1sort(lsb_mask, i1, high)};
        return {i0, i1, i2};
    }

    cube_type const * data() const noexcept {
        return the_cubes.data();
    }

    cube_type * data() noexcept {
        return the_cubes.data();
    }

    cube_type const & front() const {
        return the_cubes.front();
    }

    cube_type & front() {
        return the_cubes.front();
    }

    cube_type const & back() const {
        return the_cubes.back();
    }

    cube_type & back() {
        return the_cubes.back();
    }

    const_iterator_type cbegin() const noexcept {
        return the_cubes.cbegin();
    }

    const_iterator_type begin() const noexcept {
        return the_cubes.begin();
    }

    iterator_type begin() noexcept {
        return the_cubes.begin();
    }

    const_iterator_type cend() const noexcept {
        return the_cubes.cend();
    }

    const_iterator_type end() const noexcept {
        return the_cubes.end();
    }

    iterator_type end() noexcept {
        return the_cubes.end();
    }

    bool empty() const {
        return the_cubes.empty();
    }

    std::size_t size() const {
        return the_cubes.size();
    }

    void resize(std::size_t count) {
        the_cubes.resize(count);
    }

    cube_type const & operator[](std::size_t const pos) const {
        return the_cubes[pos];
    }

    cube_type & operator[](std::size_t const pos) {
        return the_cubes[pos];
    }

    std::size_t ninbits() const {
        return the_ninbits;
    }

    std::size_t noutbits() const {
        return the_noutbits;
    }

    inputs_value_type lookup_mask() const {
        return the_lookup_mask;
    }

    std::size_t ncubes() const {
        return size();
    }

    std::size_t nbytes() const {
        return sizeof(std::vector<cube_type>) + size() * sizeof(cube_type)
         + 2 * sizeof(std::size_t) + sizeof(inputs_value_type);
    }

    // Returns cend() if the container's size is equal to or less than n.
    const_iterator_type citer_to_nth_element(
        std::size_t const n
    ) const noexcept {
        if (size() <= n) {
            return cend();
        }
        const_iterator_type result{cbegin()};
        std::advance(result, n);
        return result;
    }

    // Returns end() if the container's size is equal to or less than n.
    const_iterator_type iter_to_nth_element(
        std::size_t const n
    ) const noexcept {
        if (size() <= n) {
            return end();
        }
        const_iterator_type result{begin()};
        std::advance(result, n);
        return result;
    }

    // Returns end() if the container's size is equal to or less than n.
    iterator_type iter_to_nth_element(std::size_t const n) noexcept {
        if (size() <= n) {
            return end();
        }
        iterator_type result{begin()};
        std::advance(result, n);
        return result;
    }

    // Intended for use immediately before serialization and after
    // deserialization whenever the cube order does not matter.  (XZ compression
    // tends to work substantially better with sorted data.)
    void ensure_sorted() {
        if (std::is_sorted(begin(), end())) {
            std::cerr << "DD: The cubes were already sorted.\n";
        } else {
            std::cerr << "II: Sorting the cubes.\n";
            std::sort(begin(), end());
        }
    }

    void sort(
        inputs_value_type const mask,
        std::size_t const low,
        std::size_t const high
    ) {
        assert(low <= high);
        std::sort(iter_to_nth_element(low), iter_to_nth_element(high),
         [mask](cube_type const & p1, cube_type const & p2) {
            return (mask & static_cast<inputs_value_type>(p1.inputs()))
             < (mask & static_cast<inputs_value_type>(p2.inputs()));
         });
    }

    void sort(inputs_value_type const mask) {
        sort(mask, 0, the_cubes.size());
    }

    // Returns true iff an error occurred.
    [[nodiscard]]
    bool serialize(std::ostream & data_out) const {
        std::size_t const cube_count{size()};
        data_out.write(reinterpret_cast<char const *>(&cube_count),
         sizeof(cube_count));
        data_out.write(reinterpret_cast<char const *>(&the_ninbits),
         sizeof(the_ninbits));
        data_out.write(reinterpret_cast<char const *>(&the_noutbits),
         sizeof(the_noutbits));
        data_out.write(reinterpret_cast<char const *>(&maxinbits),
         sizeof(maxinbits));
        data_out.write(reinterpret_cast<char const *>(&maxoutbits),
         sizeof(maxoutbits));
        std::size_t const write_size{cube_count * sizeof(cube_type)};
        assert(write_size <= std::numeric_limits<std::streamsize>::max());
        data_out.write(reinterpret_cast<char const * const>(data()),
         static_cast<std::streamsize>(write_size));
        if (data_out.fail()) {
            std::cerr << "EE: ostream failure while serializing cube list.\n";
        }
        return data_out.fail();
    }

    // Returns true iff an error occurred.
    [[nodiscard]]
    bool serialize(std::filesystem::path const & write_path) const {
        std::ofstream data_out(write_path, std::ios::out | std::ios::binary);
        if (not data_out.good()) {
            std::cerr << "EE: Could not open " << write_path
             << " for writing.\n";
            return true;
        }
        return serialize(data_out);
    }

    cube_list_tt(cube_list_tt const &) = default;
    cube_list_tt(cube_list_tt &&) = default;
    cube_list_tt & operator=(cube_list_tt const &) = default;
    cube_list_tt & operator=(cube_list_tt &&) = default;

    cube_list_tt(
        std::size_t const ninbits,
        std::size_t const noutbits
    ) : the_ninbits(ninbits),
        the_noutbits(noutbits),
        the_lookup_mask(fill_with_uint8<inputs_value_type>(UINT8_C(0xff))
         << (cube_type::input_bit_count - 2 * ninbits)
         >> (cube_type::input_bit_count - 2 * ninbits)) {}

    [[nodiscard]]
    static std::optional<cube_list_tt<MAXINBITS, MAXOUTBITS>> create_opt(
        std::istream & data_in
    ) {
        std::size_t cube_count;
        data_in.read(reinterpret_cast<char *>(&cube_count), sizeof(cube_count));
        // TODO: Clarify why ninbits and noutbits are being saved and loaded.
        std::size_t ninbits;
        data_in.read(reinterpret_cast<char *>(&ninbits), sizeof(ninbits));
        std::size_t noutbits;
        data_in.read(reinterpret_cast<char *>(&noutbits), sizeof(noutbits));
        std::size_t minbits;
        data_in.read(reinterpret_cast<char *>(&minbits), sizeof(minbits));
        std::size_t moutbits;
        data_in.read(reinterpret_cast<char *>(&moutbits), sizeof(moutbits));
        if (minbits != MAXINBITS or moutbits != MAXOUTBITS) {
            std::cerr << "EE: cube_list_tt template parameter mismatch: <"
             << minbits << ", " << moutbits << "> at save time, but <"
             << maxinbits << ", " << maxoutbits << "> at load time.\n";
            return std::nullopt;
        }
        // TODO: Consider improving the implementation to support larger lists.
        std::size_t const read_size{cube_count * sizeof(cube_type)};
        if (std::numeric_limits<std::streamsize>::max() < read_size) {
            std::cerr << "EE: Required read size exceeds limit.\n";
            return std::nullopt;
        }
        if (not good(data_in)) {
            return std::nullopt;
        }
        cube_list_tt<MAXINBITS, MAXOUTBITS> result(ninbits, noutbits);
        result.resize(cube_count);
        data_in.read(reinterpret_cast<char * const>(result.data()),
         static_cast<std::streamsize>(read_size));
        if (data_in.fail()) {
            std::cerr << "EE: A failure occurred while deserializing a cube "
             "list.\n";
            return std::nullopt;
        }
        result.the_lookup_mask = fill_with_uint8<inputs_value_type>(UINT8_C(0xff))
         << (cube_type::input_bit_count - 2 * result.ninbits())
         >> (cube_type::input_bit_count - 2 * result.ninbits());
        return result;
    }

    [[nodiscard]]
    static std::optional<cube_list_tt<MAXINBITS, MAXOUTBITS>> create_opt(
        std::filesystem::path const & read_path
    ) {
        std::error_code error_code;
        if (not std::filesystem::exists(read_path, error_code)) {
            std::cerr << "EE: " << read_path << " does not exist.\n";
            return std::nullopt;
        }
        if (data_category_enum::cube_list
         != categorize_by_extension_opt(read_path)) {
            std::cerr << "WW: " << read_path
             << " for cube list has unexpected extension.\n";
        }
        std::ifstream data_in(read_path, std::ios::in | std::ios::binary);
        if (not good(data_in)) {
            std::cerr << "EE: Could not open " << read_path
             << " for reading.\n";
            return std::nullopt;
        }
        return create_opt(data_in);
    }

    std::optional<outputs_value_type> lookup(
        cube_binary_inputs_tt<MAXINBITS> const & z,
        std::size_t const low,
        std::size_t const high
    ) const {
        for (std::size_t i{low}; i != high; ++i) {
            if ((*this)[i].inputs().contains_using_mask(z, the_lookup_mask)) {
                return {(*this)[i].outputs()};
            }
        }
        return std::nullopt;
    }

    std::optional<outputs_value_type> lookup(
        cube_binary_inputs_tt<MAXINBITS> const & z
    ) const {
        return lookup(z, 0, size());
    }

    std::optional<std::size_t> lookupindex(
        cube_binary_inputs_tt<MAXINBITS> const & z,
        std::size_t const low,
        std::size_t const high
    ) const {
        for (std::size_t i{low}; i != high; ++i) {
            if ((*this)[i].inputs().contains_using_mask(z, the_lookup_mask)) {
                return i;
            }
        }
        return std::nullopt;
    }

    std::optional<std::size_t> lookupindex(
        cube_binary_inputs_tt<MAXINBITS> const & z
    ) const {
        return lookupindex(z, 0, size());
    }

    std::set<outputs_value_type> lookupall(
        cube_binary_inputs_tt<MAXINBITS> const & z,
        std::size_t const low,
        std::size_t const high,
        std::size_t const result_size_limit = 0
    ) const {
        std::set<outputs_value_type> result;
        for (std::size_t i{low}; i != high; ++i) {
            if ((*this)[i].inputs().overlaps_using_mask(z, the_lookup_mask)) {
                result.insert((*this)[i].outputs());
                if ((0 != result_size_limit)
                 and (result_size_limit <= result.size())) {
                    break;
                }
            }
        }
        return result;
    }

    std::set<outputs_value_type> lookupall(
        cube_binary_inputs_tt<MAXINBITS> const & z,
        std::size_t const maxsz = 0
    ) const {
        return lookupall(z, 0, size(), maxsz);
    }

    std::set<std::size_t> lookupallindex(
        cube_binary_inputs_tt<MAXINBITS> const & z,
        std::size_t const low,
        std::size_t const high,
        std::size_t const result_size_limit = 0
    ) const {
        std::set<std::size_t> result;
        for (std::size_t i{low}; i != high; ++i) {
            if ((*this)[i].inputs().overlaps_using_mask(z, the_lookup_mask)) {
                result.insert(i);
                if ((0 != result_size_limit)
                 and (result_size_limit <= result.size())) {
                    break;
               }
            }
        }
        return result;
    }

    std::set<std::size_t> lookupallindex(
        cube_binary_inputs_tt<MAXINBITS> const & z,
        std::size_t const maxsz = 0
    ) const {
        return lookupallindex(z, 0, size(), maxsz);
    }

    template <typename move_constructible_fn_type>
    void for_each(move_constructible_fn_type unary_fn) const {
        serial_for_each(cbegin(), cend(), unary_fn);
    }

    template <typename move_constructible_fn_type>
    void for_each(move_constructible_fn_type unary_fn) {
        serial_for_each(begin(), end(), unary_fn);
    }

    template <typename copy_constructible_fn_type>
    void desire_parallel_for_each(copy_constructible_fn_type unary_fn) const {
        desire_parallel_for_each(cbegin(), cend(), unary_fn);
    }

    template <typename copy_constructible_fn_type>
    void desire_parallel_for_each(copy_constructible_fn_type unary_fn) {
        desire_parallel_for_each(begin(), end(), unary_fn);
    }

    template <typename copy_constructible_fn_type>
    void choose_if_parallelism_is_desirable_for_each(
        bool const use_parallelism_if_available,
        copy_constructible_fn_type unary_fn
    ) const {
        optionally_parallel_for_each(use_parallelism_if_available, cbegin(),
         cend(), unary_fn);
    }

    template <typename copy_constructible_fn_type>
    void choose_if_parallelism_is_desirable_for_each(
        bool const use_parallelism_if_available,
        copy_constructible_fn_type unary_fn
    ) {
        optionally_parallel_for_each(use_parallelism_if_available, begin(),
         end(), unary_fn);
    }
};


template <
    std::size_t const lhs_input_bit_pair_count,
    std::size_t const lhs_output_bit_count,
    std::size_t const rhs_input_bit_pair_count,
    std::size_t const rhs_output_bit_count
> bool operator==(
    cube_list_tt<lhs_input_bit_pair_count, lhs_output_bit_count> const & lhs,
    cube_list_tt<rhs_input_bit_pair_count, rhs_output_bit_count> const & rhs
) {
    bool const worth_scanning{(lhs.ninbits() == rhs.ninbits())
     and (lhs.noutbits() == rhs.noutbits())
     and (lhs.lookup_mask() == rhs.lookup_mask())
     and (lhs.size() == rhs.size())};
    if (not worth_scanning) {
        return false;
    }
    for (std::size_t i{}; i != lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}


template <
    std::size_t const lhs_input_bit_pair_count,
    std::size_t const lhs_output_bit_count,
    std::size_t const rhs_input_bit_pair_count,
    std::size_t const rhs_output_bit_count
> bool operator!=(
    cube_list_tt<lhs_input_bit_pair_count, lhs_output_bit_count> const & lhs,
    cube_list_tt<rhs_input_bit_pair_count, rhs_output_bit_count> const & rhs
) {
    return not (lhs == rhs);
}


template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> std::ostream & operator<<(
    std::ostream & out,
    cube_list_tt<input_bit_pair_count, output_bit_count> const & cubes
) {
    for (std::size_t nth_cube{}; nth_cube != cubes.size(); ++nth_cube) {
        out << "[" << std::setw(11) << nth_cube << "] = " << cubes[nth_cube]
         << "\n";
    }
    return out;
}


#endif // EGT_COMPARISON_CUBE_LIST_TYPE_HPP
