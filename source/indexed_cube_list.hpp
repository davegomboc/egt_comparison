#ifndef EGT_COMPARISON_INDEXED_CUBE_LIST_HPP
#define EGT_COMPARISON_INDEXED_CUBE_LIST_HPP


#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <set>
#include <system_error>
#include <utility>
#include <vector>

#include "bit_stuff.hpp"
#include "cube_binary_inputs.hpp"
#include "cube_list.hpp"
#include "generic_tree.hpp"
#include "logic_digit_enum.hpp"
#include "parallel_for_each.hpp"
#include "uint_type_selection.hpp"


template <std::size_t const required_index_bit_count>
struct cube_list_index_range_tt {
    using index_type = selected_uint_tt<required_index_bit_count>;
    cube_list_index_range_tt(index_type l, index_type h) : low{l}, high{h} {}

    std::string to_string() const {
        return std::to_string(low) + "," + std::to_string(high) + " ["
         + std::to_string(high - low) + "]";
    }

    // The range is half-open: [low, high)
    index_type low, high;
};


// required_cube_list_index_bit_count = # bits needed for the cube list: choose
// such that 2^required_cube_list_index_bit_count > # of cubes in cube list
//
// required_tree_node_index_bit_count = # bits needed for the index tree: choose
// such that 2^required_tree_node_index_bit_count > # of nodes in tree
template <
    typename cube_list_maybe_ref_type,
    std::size_t const required_cube_list_index_bit_count,
    std::size_t const required_tree_node_index_bit_count
> class indexed_cube_list_tt {
    using cube_list_non_ref_type
     = std::remove_reference_t<cube_list_maybe_ref_type>;
    using generic_tree_type = generic_tree_tt<
        cube_list_index_range_tt<required_cube_list_index_bit_count>,
        selected_uint_tt<cube_list_non_ref_type::maxinbits>,
        required_tree_node_index_bit_count,
        3
    >;
    using dtype = selected_uint_tt<cube_list_non_ref_type::maxinbits>;
    using node_index_type = typename generic_tree_type::node_index_type;

public:
    using cube_type = typename cube_list_non_ref_type::cube_type;
    using itype = typename cube_list_index_range_tt<
     required_cube_list_index_bit_count>::index_type;

    using iterator_type = typename std::vector<cube_type>::iterator;
    using const_iterator_type = typename std::vector<cube_type>::const_iterator;

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

    cube_type const & operator[](std::size_t const pos) const {
        return the_cubes[pos];
    }

    cube_type & operator[](std::size_t const pos) {
        return the_cubes[pos];
    }

    // Quick tests suggest that 10-20 is about right for maxperleaf.  16 seems
    // like a nice number.
    //
    // make_simple_quick_split_choices just orders the index in bit order -- it
    // turns out to be faster to build and the same (or faster) to query
    // usually, without being larger, so the default is to avoid all of the
    // complex logic to decide which bit to split on next
    template <
        typename U = cube_list_maybe_ref_type,
        std::enable_if_t<std::is_reference_v<U>, int> = 0
    > indexed_cube_list_tt(
        cube_list_non_ref_type & clst,
        bool const make_simple_quick_split_choices = true,
        itype const maxperleaf = 16
    ) : the_cubes(clst) {
        assert(the_cubes.size() < std::numeric_limits<itype>::max());
        std::vector<dtype> dleft(the_cubes.ninbits());
        std::iota(dleft.begin(), dleft.end(), 0);
        the_generic_tree.set_root_node_index(build_tree(0,
         static_cast<itype>(the_cubes.size()), dleft, dleft.size(), maxperleaf,
         make_simple_quick_split_choices));
    }
    template <
        typename U = cube_list_maybe_ref_type,
        std::enable_if_t<not std::is_reference_v<U>, int> = 0
    > indexed_cube_list_tt(
        cube_list_non_ref_type clst,
        bool const make_simple_quick_split_choices = true,
        itype const maxperleaf = 16
    ) : the_cubes(std::move(clst)) {
        assert(the_cubes.size() < std::numeric_limits<itype>::max());
        std::vector<dtype> dleft(the_cubes.ninbits());
        std::iota(dleft.begin(), dleft.end(), 0);
        the_generic_tree.set_root_node_index(build_tree(0,
         static_cast<itype>(the_cubes.size()), dleft, dleft.size(), maxperleaf,
         make_simple_quick_split_choices));
    }

    // Returns true iff an error occurred.
    [[nodiscard]]
    bool serialize(std::ostream & data_out) const {
        bool result{the_generic_tree.serialize(data_out)};
        result |= the_cubes.serialize(data_out);
        return result;
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

    [[nodiscard]]
    static std::optional<indexed_cube_list_tt<
        cube_list_maybe_ref_type,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    >> create_opt(std::istream & data_in) {
        std::optional<generic_tree_type> generic_tree_opt{
         generic_tree_type::create_opt(data_in)};
        std::optional<cube_list_non_ref_type> cubes_opt{
         cube_list_non_ref_type::create_opt(data_in)};
        if (not generic_tree_opt or not cubes_opt) {
            return std::nullopt;
        }
        indexed_cube_list_tt<
            cube_list_maybe_ref_type,
            required_cube_list_index_bit_count,
            required_tree_node_index_bit_count
        > result(cubes_opt->ninbits(), cubes_opt->noutbits());
        result.the_generic_tree = std::move(*generic_tree_opt);
        result.the_cubes = std::move(*cubes_opt);
        return result;
    }

    [[nodiscard]]
    static std::optional<indexed_cube_list_tt<
        cube_list_maybe_ref_type,
        required_cube_list_index_bit_count,
        required_tree_node_index_bit_count
    >> create_opt(std::filesystem::path const & read_path) {
        std::error_code error_code;
        if (not std::filesystem::exists(read_path, error_code)) {
            std::cerr << "EE: " << read_path << " does not exist.\n";
            return std::nullopt;
        }
        if (data_category_enum::indexed_cube_list
         != categorize_by_extension_opt(read_path)) {
            std::cerr << "WW: " << read_path
             << " for indexed cube list has unexpected extension.\n";
        }
        std::ifstream data_in(read_path, std::ios::in | std::ios::binary);
        if (not data_in.good()) {
            std::cerr << "EE: Could not open " << read_path
             << " for reading.\n";
            return std::nullopt;
        }
        return create_opt(data_in);
    }

private:
    indexed_cube_list_tt(
        std::size_t const ninbits,
        std::size_t const noutbits
    ) : the_cubes(ninbits, noutbits) {}

    static auto getoutvalue(
        indexed_cube_list_tt const & icl,
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        node_index_type const n
    ) {
        return icl.the_cubes.lookup(z,
         icl.the_generic_tree.as_leaf_value(n).low,
         icl.the_generic_tree.as_leaf_value(n).high);
    }
    static auto getindex(
        indexed_cube_list_tt const & icl,
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        node_index_type const n
    ) {
        return icl.the_cubes.lookupindex(z,
         icl.the_generic_tree.as_leaf_value(n).low,
         icl.the_generic_tree.as_leaf_value(n).high);
    }
    static auto getalloutvalue(
        indexed_cube_list_tt const & icl,
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        node_index_type const n,
        std::size_t const result_size_limit
    ) {
        return icl.the_cubes.lookupall(z,
         icl.the_generic_tree.as_leaf_value(n).low,
         icl.the_generic_tree.as_leaf_value(n).high, result_size_limit);
    }
    static auto getallindex(
        indexed_cube_list_tt const & icl,
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        node_index_type const n,
        std::size_t const result_size_limit
    ) {
        return icl.the_cubes.lookupallindex(z,
         icl.the_generic_tree.as_leaf_value(n).low,
         icl.the_generic_tree.as_leaf_value(n).high, result_size_limit);
    }

public:
    typename cube_list_non_ref_type::outputs_value_type lookup(
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z
    ) const {
        auto const temp{lookup_impl(z, the_generic_tree.root_node_index(),
         getoutvalue)};
        if (std::nullopt == temp) {
            return 0;
        }
        return *temp;
    }

    std::size_t lookupindex(
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z
    ) const {
        return *(lookup_impl(z, the_generic_tree.root_node_index(), getindex));
    }

    std::set<typename cube_list_non_ref_type::outputs_value_type> lookupall(
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        std::size_t const result_size_limit = 0
    ) const {
        return lookupallimpl(z, the_generic_tree.root_node_index(),
         getalloutvalue, result_size_limit);
    }

    std::set<std::size_t> lookupallindex(
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        std::size_t const result_size_limit = 0
    ) const {
        return lookupallimpl(z, the_generic_tree.root_node_index(), getallindex,
         result_size_limit);
    }

    std::size_t ncubes() const {
        return the_cubes.ncubes();
    }
    std::size_t nbytes() const {
        return the_cubes.nbytes() + the_generic_tree.nbytes();
    }
    std::size_t nnodes() const {
        return the_generic_tree.nnodes();
    }
    std::size_t nleaves() const {
        return the_generic_tree.nleaves();
    }

    cube_list_maybe_ref_type const & cubes() const {
        return the_cubes;
    }

    auto to_string() const {
        return the_generic_tree.to_string(
         [](dtype const d) {
             return std::to_string(static_cast<unsigned int const>(d));
         },
         [](cube_list_index_range_tt<required_cube_list_index_bit_count> const r) {
             return r.to_string();
         });
    }

    template <bool const emit_tracing = false>
    bool input_variable_of_cube_is_raisable(
        cube_type const & cube,
        typename cube_type::inputs_type::index_type nth_bit_pair
    ) const {
        if (emit_tracing) {
            std::cerr << "DD: Entered input_variable_of_cube_is_raisable().\n"
             "TT:           cube = " << cube << "\n"
             "TT:   nth_bit_pair = " << nth_bit_pair << unsigned(nth_bit_pair);
            std::cerr.put('\n');
        }
        logic_digit_enum const logic_digit{cube.inputs()[nth_bit_pair]};
        if (emit_tracing) {
            std::cerr << "TT:    logic_digit = " << logic_digit << '\n';
        }
        if ((lde_dc == logic_digit) or (lde_sng == logic_digit)) {
            if (emit_tracing) {
                std::cerr << "DD: Exiting input_variable_of_cube_is_raisable(),"
                 " returning false.\n";
            }
            return false;
        }

        typename cube_type::inputs_type expansion_zone{cube.inputs()};
        expansion_zone.set(nth_bit_pair, flipped(expansion_zone[nth_bit_pair]));
        if (emit_tracing) {
            std::cerr << "TT: Any care output must be "
             << static_cast<unsigned int>(cube.outputs())
             << "\nTT: expansion_zone = " << expansion_zone << '\n';
        }
        std::set<typename cube_type::outputs_value_type> const
         outputs_within_expansion_zone{lookupall(expansion_zone, 2)};
        if (1 < outputs_within_expansion_zone.size()) {
            if (emit_tracing) {
                std::cerr << "TT: Multiple distinct care outputs found.\n"
                 "DD: Exiting input_variable_of_cube_is_raisable(), returning "
                 "false.\n";
            }
            return false;
        }
        if (outputs_within_expansion_zone.empty()) {
            if (emit_tracing) {
                std::cerr << "TT: The expansion zone is entirely don't care.\nDD: "
                 "Exiting input_variable_of_cube_is_raisable(), returning true.\n";
            }
            return true;
        }
        assert(1 == outputs_within_expansion_zone.size());
        bool const matches{
         cube.outputs() == *(outputs_within_expansion_zone.cbegin())};
        if (emit_tracing) {
            std::cerr << "DD: Exiting input_variable_of_cube_is_raisable(), "
             "returning " << (matches ? "true" : "false") << ".\n";
        }
        return matches;
    }

    template <bool const emit_tracing = false>
    void attempt_raising(
        cube_type & cube,
        std::vector<typename cube_type::inputs_index_type> const &
         raise_attempt_order
    ) const {
        if (emit_tracing) {
            std::cerr << "DD: Entered attempt_raising().\n";
            std::cerr << "TT:  cube = " << cube << '\n';
        }
        for (typename cube_type::inputs_index_type const index:
         raise_attempt_order) {
            if (emit_tracing) {
                std::cerr << "TT: index = " << unsigned(index) << '\n';
            }
            if (input_variable_of_cube_is_raisable<emit_tracing>(cube, index)) {
                cube.inputs().raise(index);
                if (emit_tracing) {
                    std::cerr << "TT:  cube = " << cube << '\n';
                }
            }
        }
        if (emit_tracing) {
            std::cerr << "DD: Exiting attempt_raising().\n";
        }
    }

private:
    generic_tree_type the_generic_tree;
    cube_list_maybe_ref_type the_cubes;

    //using score_type = std::size_t;
    using score_type = double;
    using count_type = std::size_t;

    node_index_type build_tree(
        itype const low,
        itype const high,
        std::vector<dtype> & dleft,
        std::size_t const dn,
        itype const maxleaf,
        bool const make_simple_quick_split_choices
    ) {
        if ((0 == dn) or (high - low <= maxleaf)) {
            return the_generic_tree.add_leaf(cube_list_index_range_tt<
             required_cube_list_index_bit_count>(low, high));
        }

        decltype(dleft.end() - dleft.begin()) besti;
        // Pick the dimension to split on.
        if (make_simple_quick_split_choices) {
            besti = 0;
        } else {
            std::vector<std::size_t> indices(dn);
            std::iota(indices.begin(), indices.end(), 0);
            std::vector<score_type> scores(dn, 0.0);
            desire_parallel_for_each(indices.begin(), indices.end(),
             [&](auto const i) {
                //scores[i] = scoredim(low, high, dleft[i]);
                scores[i] = scoredim2(low, high, dleft[i]);
             });
            besti = std::min_element(scores.begin(),
             scores.end()) - scores.begin();
            assert(0 <= besti);
        }
        std::swap(dleft[dn - 1], dleft[static_cast<std::size_t>(besti)]);
        dtype bestd = dleft[dn - 1];

        auto [i0, off_high, on_high] = the_cubes.sort1bit(bestd, low, high);
        return the_generic_tree.add_internal(bestd, {
            build_tree(low, static_cast<itype>(off_high), dleft, dn - 1,
             maxleaf, make_simple_quick_split_choices),
            build_tree(static_cast<itype>(off_high),
             static_cast<itype>(on_high), dleft, dn - 1, maxleaf,
             make_simple_quick_split_choices),
            build_tree(static_cast<itype>(on_high), high, dleft, dn - 1,
             maxleaf, make_simple_quick_split_choices)
        });
    }

    std::array<count_type, 4> split_histogram(
        itype const low,
        itype const high,
        dtype const d
    ) {
        std::array<count_type, 4> result{0, 0, 0, 0};
        using local_itype = typename cube_list_non_ref_type::inputs_index_type;
        assert(d <= std::numeric_limits<local_itype>::max());
        for (itype i = low; i < high; i++) {
            logic_digit_enum const bit_pair{
             the_cubes[i].inputs()[static_cast<local_itype>(d)]};
            ++result[as_uchar(bit_pair)];
        }
        return result;
    }

    score_type scoredim(itype const low, itype const high, dtype const d) {
        auto num = split_histogram(low, high, d);
        return static_cast<score_type>(
         num[1] * num[1] + num[2] * num[2] + num[3] * (high - low + num[3]));
    }

    score_type scoredim2(itype const low, itype const high, dtype const d) {
        auto num = split_histogram(low, high, d);
        return static_cast<score_type>(
         std::log(num[1] + num[3]) + std::log(num[2] + num[3]));
    }

    template <typename TORET>
    auto lookup_impl(
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        node_index_type const n,
        TORET toret
    ) const -> decltype(toret(*this, z, n)) {
        if (the_generic_tree.is_leaf(n)) {
            return toret(*this, z, n);
        }
        // Check "on" or "off" branch first (querying all of the original
        // minterms of KRN_vs_kp against an indexed_cube_list_tt of D2-merge
        // version of the same this (on/off first) is much faster (4-5 times
        // faster).
        logic_digit_enum const bit_pair{z[static_cast<typename
         cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits>::index_type>(
         the_generic_tree.as_internal_value(n))]};
        switch (bit_pair) {
        case lde_off:
            {
                auto ret = lookup_impl(z, the_generic_tree.branch(n, 0), toret);
                if (ret) {
                    return ret;
                }
                break;
            }
        case lde_on:
            {
                auto ret = lookup_impl(z, the_generic_tree.branch(n, 1), toret);
                if (ret) {
                    return ret;
                }
                break;
            }
        case lde_sng:
        case lde_dc:
        default:
            break;
        }
        return lookup_impl(z, the_generic_tree.branch(n, 2), toret);
    }

    template <typename TORET>
    auto lookupallimpl(
        cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits> const & z,
        node_index_type const n,
        TORET toret,
        std::size_t const result_size_limit = 0
    ) const -> decltype(toret(*this, z, n, result_size_limit)) {
        if (the_generic_tree.is_leaf(n)) {
            return toret(*this, z, n, result_size_limit);
        }
        auto ret = lookupallimpl(z, the_generic_tree.branch(n, 2), toret,
         result_size_limit);
        if ((0 < result_size_limit) and (result_size_limit <= ret.size())) {
            return ret;
        }
        logic_digit_enum const bit_pair{z[static_cast<typename
         cube_binary_inputs_tt<cube_list_non_ref_type::maxinbits>::index_type>(
         the_generic_tree.as_internal_value(n))]};
        if ((lde_off == bit_pair) or (lde_dc == bit_pair)) {
            auto ret2 = lookupallimpl(z, the_generic_tree.branch(n, 0), toret,
             result_size_limit);
            ret.insert(ret2.begin(), ret2.end());
            if ((0 != result_size_limit)
             and (result_size_limit <= ret.size())) {
                return ret;
            }
        }
        if ((lde_on == bit_pair) or (lde_dc == bit_pair)) {
            auto ret2 = lookupallimpl(z, the_generic_tree.branch(n, 1), toret,
             result_size_limit);
            ret.insert(ret2.begin(), ret2.end());
        }
        return ret;
    }
};


#endif // EGT_COMPARISON_INDEXED_CUBE_LIST_HPP
