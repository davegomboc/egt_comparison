#ifndef EGT_COMPARISON_LOAD_AND_SAVE_HPP
#define EGT_COMPARISON_LOAD_AND_SAVE_HPP


#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>

#include "cube_list.hpp"
#include "data_category_enum.hpp"
#include "decision_dag.hpp"
#include "filesystem_stuff.hpp"
#include "indexed_cube_list.hpp"
#include "uint_type_selection.hpp"


// TODO: Once we've upgraded to C++20, use std::remove_cvref_t directly.
template <typename T>
struct remove_cvref {
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;


// Attempts to find where chesspresso binary files could be stored on disk.
// Returns std::nullopt when it doesn't have a good guess.
inline std::optional<std::filesystem::path> chesspresso_directory_path_opt(
    std::string_view const material_balance
) {
    std::error_code error_code;
    char const * const egt_path_as_c_str{std::getenv("EGT_DIR")};
    if (egt_path_as_c_str) {
        std::filesystem::path candidate_directory_path(egt_path_as_c_str);
        candidate_directory_path /= "chesspresso";
        candidate_directory_path /= std::to_string(
         material_balance.size() - 4 /* _vs_ */);
        if (std::filesystem::exists(candidate_directory_path, error_code)) {
            return candidate_directory_path;
        }
    }
    return std::nullopt;
}

// Attempts to find where the chesspresso binary file either is or ought to be
// located on disk.  Returns std::nullopt when it doesn't have a good guess.
inline std::optional<std::filesystem::path> chesspresso_path_opt(
    std::string_view const material_balance
) {
    std::optional<std::filesystem::path> directory_path_opt{
     chesspresso_directory_path_opt(material_balance)};
    if (std::nullopt == directory_path_opt) {
        return std::nullopt;
    }
    std::filesystem::path result{*directory_path_opt};
    result /= material_balance;
    result += '.';
    result += extension(data_category_enum::chesspresso_binary);
    return result;
}

// Attempts to find where decision DAGs could be stored on disk.  Returns
// std::nullopt when it doesn't have a good guess.
inline std::optional<std::filesystem::path>
decision_dags_directory_path_opt(std::string_view const material_balance) {
    std::error_code error_code;
    char const * const egt_path_as_c_str{std::getenv("EGT_DIR")};
    if (egt_path_as_c_str) {
        std::filesystem::path candidate_directory_path(egt_path_as_c_str);
        candidate_directory_path /= "decision_dags";
        candidate_directory_path /= std::to_string(
         material_balance.size() - 4 /* _vs_ */);
        if (std::filesystem::exists(candidate_directory_path, error_code)) {
            return candidate_directory_path;
        }
    }
    return std::nullopt;
}

// Attempts to find where the decision DAG file either is or ought to be located
// on disk.  Returns std::nullopt when it doesn't have a good guess.
inline std::optional<std::filesystem::path>
decision_dag_path_opt(std::string_view const material_balance) {
    std::error_code error_code;
    std::optional<std::filesystem::path> directory_path_opt{
     decision_dags_directory_path_opt(material_balance)};
    if (std::nullopt == directory_path_opt) {
        return std::nullopt;
    }
    std::filesystem::path result{*directory_path_opt};
    result /= material_balance;
    result += '.';
    result += extension(data_category_enum::ddag);
    return result;
}

inline std::ifstream open_file_read(std::filesystem::path const & file) {
    std::ifstream result(file, std::ios::in | std::ios::binary);
    if (not result.good()) {
        std::cerr << "EE: Could not open " << file << " for reading.\n";
        std::exit(EXIT_FAILURE);
    }
    return result;
}

inline std::ofstream open_file_write(std::filesystem::path const & file) {
    std::ofstream result(file, std::ios::out | std::ios::binary);
    if (not result.good()) {
        std::cerr << "EE: Could not open " << file << " for writing.\n";
        std::exit(EXIT_FAILURE);
    }
    return result;
}

template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
>
void save_cube_list(
    std::filesystem::path const & preexisting_save_directory_path,
    std::string_view const material_balance,
    std::string_view const proc,
    cube_list_tt<input_bit_pair_count, output_bit_count> const & cube_list
) {
    std::error_code error_code;
    if (not std::filesystem::exists(preexisting_save_directory_path,
     error_code)) {
        std::cerr << "EE: The save directory "
         << preexisting_save_directory_path << " does not already exist.\n";
        std::exit(EXIT_FAILURE);
    }
    std::filesystem::path const save_path(data_file_path(
     preexisting_save_directory_path, data_category_enum::cube_list,
     material_balance, proc));
    bool const write_failed{cube_list.serialize(save_path)};
    if (write_failed) {
        std::exit(EXIT_FAILURE);
    }
}

template <
    std::size_t const input_bit_pair_count,
    std::size_t const output_bit_count
> auto load_cube_list_opt(
    std::string_view const material_balance,
    std::string_view const proc
) {
    std::optional<cube_list_tt<input_bit_pair_count, output_bit_count>> result;
    std::optional<std::filesystem::path> path_opt(
     cube_list_path_opt(material_balance, proc, false));
    if (path_opt) {
        result = cube_list_tt<input_bit_pair_count, output_bit_count>
         ::create_opt(*path_opt);
    }
    return result;
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
void save_ddag(
    std::ostream & data_out,
    ddag<MAXINBITS, MAXOUTBITS, INDEXBITS> const & dt
) {
    dt.serialize(data_out);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
void save_ddag(
    std::filesystem::path const & save_path,
    ddag<MAXINBITS, MAXOUTBITS, INDEXBITS> const & dt
) {
    std::ostream && data_out(open_file_write(save_path));
    save_ddag(data_out, dt);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
void save_ddag(
    std::filesystem::path const & preexisting_save_directory_path,
    std::string_view const material_balance,
    ddag<MAXINBITS, MAXOUTBITS, INDEXBITS> const & dt
) {
    std::error_code error_code;
    if (not std::filesystem::exists(preexisting_save_directory_path,
     error_code)) {
        std::cerr << "EE: The save directory "
         << preexisting_save_directory_path << " does not already exist.\n";
        std::exit(EXIT_FAILURE);
    }
    std::filesystem::path const save_path(data_file_path(
     preexisting_save_directory_path, data_category_enum::ddag,
     material_balance, ""));
    save_ddag(save_path, dt);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
auto load_ddag(std::istream & data_in) {
    return ddag<MAXINBITS, MAXOUTBITS, INDEXBITS>(data_in);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
auto load_ddag_opt(std::filesystem::path const & path) {
    std::error_code error_code;
    std::optional<ddag<MAXINBITS, MAXOUTBITS, INDEXBITS>> result;
    if (std::filesystem::exists(path, error_code)) {
        std::istream && data_in(open_file_read(path));
        result = std::make_optional(
         load_ddag<MAXINBITS, MAXOUTBITS, INDEXBITS>(data_in));
    }
    return result;
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, std::size_t INDEXBITS>
auto load_ddag_opt(std::string_view const material_balance) {
    std::optional<ddag<MAXINBITS, MAXOUTBITS, INDEXBITS>> result;
    std::optional<std::filesystem::path> path_opt(
     decision_dag_path_opt(material_balance));
    if (path_opt) {
        result = load_ddag_opt<MAXINBITS, MAXOUTBITS, INDEXBITS>(*path_opt);
    }
    return result;
}

template <
    std::size_t MAXINBITS,
    std::size_t MAXOUTBITS,
    std::size_t NINDEXBITS,
    std::size_t NTREEINDEXBITS
> void save_indexed_cube_list(
    std::filesystem::path const & preexisting_save_directory_path,
    std::string_view const material_balance,
    std::string_view const proc,
    indexed_cube_list_tt<cube_list_tt<MAXINBITS, MAXOUTBITS>, NINDEXBITS,
     NTREEINDEXBITS> const & icl) {
    std::error_code error_code;
    if (not std::filesystem::exists(preexisting_save_directory_path,
     error_code)) {
        std::cerr << "EE: The save directory "
                  << preexisting_save_directory_path
                  << " does not already exist.\n";
        std::exit(EXIT_FAILURE);
    }
    std::filesystem::path const save_path(data_file_path(
     preexisting_save_directory_path, data_category_enum::indexed_cube_list,
     material_balance, proc));
    save_cube_list(save_path, icl);
}

template <
    std::size_t const MAXINBITS = 32,
    std::size_t const MAXOUTBITS = 8,
    std::size_t const NINDEXBITS = 30,
    std::size_t const NTREEINDEXBITS = 30
> std::optional<
    indexed_cube_list_tt<
        cube_list_tt<MAXINBITS, MAXOUTBITS>,
        NINDEXBITS,
        NTREEINDEXBITS
    >
> load_indexed_cube_list_opt(
    std::string_view const material_balance,
    std::string_view const proc
) {
    std::optional<std::filesystem::path> path_opt{
     cube_list_path_opt(material_balance, proc, true)};
    if (path_opt) {
        return indexed_cube_list_tt<cube_list_tt<MAXINBITS, MAXOUTBITS>, NINDEXBITS,
         NTREEINDEXBITS>::create_opt(*path_opt);
    } else {
        return std::nullopt;
    }
}

template <std::size_t WRITEBITS, typename L, typename ostream_type>
void write_F_cubes(L const & lst, ostream_type && data_out) {
    using inputs_value_type
     = typename remove_cvref_t<L>::cube_type::inputs_value_type;
    for (auto const & p: lst) {
        selected_uint_tt<WRITEBITS> b{};
        b = p.outputs();
        b <<= 2 * lst.ninbits();
        b |= static_cast<inputs_value_type>(p.inputs());
        data_out.write(reinterpret_cast<char *>(&b), sizeof(b));
    }
}

template <std::size_t WRITEBITS, typename L, typename ostream_type>
void write_R_cubes(
    L const & lst,
    ostream_type && data_out,
    unsigned int output_bit_count = 3
) {
    using inputs_value_type
     = typename remove_cvref_t<L>::cube_type::inputs_value_type;
    for (auto const & p: lst) {
        selected_uint_tt<WRITEBITS> b{};
        b = p.outputs();
        b ^= (1u << output_bit_count) - 1;
        b <<= 2 * lst.ninbits();
        b |= static_cast<inputs_value_type>(p.inputs());
        data_out.write(reinterpret_cast<char *>(&b), sizeof(b));
    }
}

template <
    std::size_t READBITS,
    std::size_t MAXOUTBITS,
    typename L,
    typename R,
    typename SZ
> void read_F_cubes(L && lst, R && reader, SZ n) {
    selected_uint_tt<READBITS> buf;

    using inputs_value_type
     = typename remove_cvref_t<L>::cube_type::inputs_value_type;
    using outputs_value_type
     = typename remove_cvref_t<L>::cube_type::outputs_value_type;

    inputs_value_type inmask = ~(~(static_cast<inputs_value_type>(0))
     >> (2 * lst.ninbits()) << (2 * lst.ninbits()));
    //std::cerr << "TT: inmask = " << inmask << '\n';
    std::size_t const outbitshift{2 * lst.ninbits()};

    // used to read in a subset for testing if != 1
    std::size_t constexpr skipn{1};
    lst.resize(n / skipn);
    std::size_t j{};
    //std::cerr << "II: Loading " << n << " cubes.\n";
    for (std::size_t i{}; i != n; ++i) {
        reader(reinterpret_cast<char *>(&buf), sizeof(buf));
        if (i % skipn == 0) {
            lst[j].inputs() = static_cast<inputs_value_type>(buf & inmask);
            auto const temp(
             (buf >> outbitshift) & ((0x1 << (MAXOUTBITS + 1)) - 1));
            assert(temp <= std::numeric_limits<outputs_value_type>::max());
            lst[j].outputs() = static_cast<outputs_value_type>(temp);
            //std::cerr << "TT: lst[" << j << "] = " << lst[j] << '\n';
            ++j;
        }
    }
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS>
void save_chesspresso_FR_plb(
    std::filesystem::path const & output_path,
    cube_list_tt<MAXINBITS, MAXOUTBITS> const & F_cube_list,
    cube_list_tt<MAXINBITS, MAXOUTBITS> const & R_cube_list
) {
    std::ofstream data_out(open_file_write(output_path));
    save_chesspresso_FR_plb(data_out, F_cube_list, R_cube_list);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS>
void save_chesspresso_FR_plb(
    std::ostream & data_out,
    cube_list_tt<MAXINBITS, MAXOUTBITS> const & F_cube_list,
    cube_list_tt<MAXINBITS, MAXOUTBITS> const & R_cube_list
) {
    if ((F_cube_list.ninbits() != R_cube_list.ninbits())
     or (F_cube_list.noutbits() != R_cube_list.noutbits())) {
        std::cerr << "EE: F and R do not have compatible dimensions.\n"
         "II: F_cube_list.ninbits() = " << F_cube_list.ninbits()
         << "\nII: R_cube_list.ninbits() = " << R_cube_list.ninbits()
         << "\nII: F_cube_list.noutbits() = " << F_cube_list.noutbits()
         << "\nII: R_cube_list.noutbits() = " << R_cube_list.noutbits() << "\n";
        std::exit(EXIT_FAILURE);
    }
    std::uint64_t const per_cube_bit_count(
     2 * F_cube_list.ninbits() + F_cube_list.noutbits());
    std::size_t const required_uint64s_per_cube{(per_cube_bit_count + 63) / 64};
    if (2 < required_uint64s_per_cube) {
        std::cerr << "EE: Not yet configured for such large output bitsets.\n";
        std::exit(EXIT_FAILURE);
    }
    data_out.write(reinterpret_cast<char const *>(&per_cube_bit_count),
     sizeof(per_cube_bit_count));
    std::uint64_t const F_cube_count{F_cube_list.size()};
    data_out.write(reinterpret_cast<char const *>(&F_cube_count),
     sizeof(F_cube_count));
    std::uint64_t const R_cube_count{R_cube_list.size()};
    data_out.write(reinterpret_cast<char const *>(&R_cube_count),
     sizeof(R_cube_count));
    std::uint64_t const D_cube_count{};
    data_out.write(reinterpret_cast<char const *>(&D_cube_count),
     sizeof(D_cube_count));
    switch (required_uint64s_per_cube) {
    case 1:
        write_F_cubes<64>(F_cube_list, data_out);
        write_R_cubes<64>(R_cube_list, data_out);
        break;
    case 2:
        write_F_cubes<128>(F_cube_list, data_out);
        write_R_cubes<128>(R_cube_list, data_out);
        break;
    default:
        // TODO: Handle the degenerate case where per_cube_bit_count is zero.
        assert(false);
        __builtin_unreachable();
    }
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS, typename R>
cube_list_tt<MAXINBITS, MAXOUTBITS> load_chesspresso_F_plb_impl(
    R && reader,
    unsigned int const output_bit_count = 3
) {
    if (MAXOUTBITS < output_bit_count) {
        std::cerr << "EE: At least " << output_bit_count
         << "output bits are required, but only " << MAXOUTBITS
         << (1 == MAXOUTBITS ? " is" : " are") << " being used.\n";
        std::exit(EXIT_FAILURE);
    }

    std::uint64_t per_cube_bit_count{std::numeric_limits<std::uint64_t>::max()};
    reader(
     reinterpret_cast<char *>(&per_cube_bit_count), sizeof(per_cube_bit_count));
    if (128 < per_cube_bit_count) {
        std::cerr << "EE: per_cube_bit_count of " << per_cube_bit_count
         << " is unsupported.\n";
        std::exit(EXIT_FAILURE);
    }
    std::uint64_t F_cube_count;
    reader(reinterpret_cast<char *>(&F_cube_count), sizeof(F_cube_count));
    std::uint64_t R_cube_count;
    reader(reinterpret_cast<char *>(&R_cube_count), sizeof(R_cube_count));
    std::uint64_t D_cube_count;
    reader(reinterpret_cast<char *>(&D_cube_count), sizeof(D_cube_count));

    std::size_t const ninbits{(per_cube_bit_count - output_bit_count) / 2};
    if (MAXINBITS < ninbits) {
        std::cerr << "EE: At least " << ninbits
         << " input bit pairs are required, but only " << MAXINBITS
         << (1 == MAXINBITS ? " is" : " are") << " being used.\n";
        std::exit(EXIT_FAILURE);
    }
    cube_list_tt<MAXINBITS, MAXOUTBITS> result(ninbits, output_bit_count);

    //std::cerr << "II: Reading F.\n";
    std::size_t const nreadwords{
     (2 * result.ninbits() + result.noutbits() + 63) / 64};
    if (2 < nreadwords) {
        std::cerr << "EE: not yet configured for such large input bitsets.\n";
        std::exit(EXIT_FAILURE);
    }

    //std::cerr << "II: nreadwords = " << nreadwords << '\n';
    switch (nreadwords) {
    case 1:
        read_F_cubes<64, MAXOUTBITS>(result, reader, F_cube_count);
        break;
    case 2:
        read_F_cubes<128, MAXOUTBITS>(result, reader, F_cube_count);
        break;
    default:
        // TODO: Handle the degenerate case where per_cube_bit_count is zero.
        assert(false);
        __builtin_unreachable();
    }
    //std::cerr << "II: Done reading F.\n";

    return result;
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS>
cube_list_tt<MAXINBITS, MAXOUTBITS> load_chesspresso_F_plb(
    std::istream & data_in,
    unsigned int const output_bit_count = 3
) {
    return load_chesspresso_F_plb_impl<MAXINBITS, MAXOUTBITS>(
     [&data_in](char * buf, std::size_t nbytes) {
         assert(nbytes <= std::numeric_limits<std::streamsize>::max());
         data_in.read(buf, static_cast<std::streamsize>(nbytes));
     },
     output_bit_count);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS>
cube_list_tt<MAXINBITS, MAXOUTBITS> load_chesspresso_F_plb(
    std::string_view const fname,
    unsigned int const output_bit_count = 3
) {
    std::istream && data_in(open_file_read(fname));
    return load_chesspresso_F_plb_impl<MAXINBITS, MAXOUTBITS>(
     [&data_in](char * buf, std::size_t nbytes) {
         assert(nbytes <= std::numeric_limits<std::streamsize>::max());
         data_in.read(buf, static_cast<std::streamsize>(nbytes));
     },
     output_bit_count);
}

template <std::size_t MAXINBITS, std::size_t MAXOUTBITS>
cube_list_tt<MAXINBITS, MAXOUTBITS> load_chesspresso_F_plb_from_archive(
    std::string_view const material_balance,
    unsigned int const output_bit_count = 3,
    std::optional<std::filesystem::path> const & specified_archive_path_opt
     = std::nullopt,
    std::optional<std::filesystem::path> const &
    specified_extraction_script_path_opt = std::nullopt
) {
    std::filesystem::path archive_path;
    if (std::nullopt == specified_archive_path_opt) {
        std::cerr << "EE: An archive path must be specified.\n";
        std::exit(EXIT_FAILURE);
    }
    archive_path = *specified_archive_path_opt;

    std::filesystem::path extraction_script_path;
    if (std::nullopt == specified_extraction_script_path_opt) {
        std::cerr << "EE: No available extraction script path.\n";
        std::exit(EXIT_FAILURE);
    }
    extraction_script_path = *specified_extraction_script_path_opt;

    std::optional<std::filesystem::path> canonical_archive_path_opt{
     canonical_path_opt(archive_path)};
    if (std::nullopt == canonical_archive_path_opt) {
        std::cerr << "EE: " << archive_path << " was not canonicalizable.\n";
        std::error_code error_code;
        std::filesystem::path current_working_directory{
         std::filesystem::current_path(error_code)};
        std::cerr << "II: " << current_working_directory << '\n';
        std::exit(EXIT_FAILURE);
    }
    archive_path = *canonical_archive_path_opt;

    std::optional<std::filesystem::path> canonical_extraction_script_path_opt{
     canonical_path_opt(extraction_script_path)};
    if (std::nullopt == canonical_extraction_script_path_opt) {
        std::cerr << "EE: " << extraction_script_path
         << " was not canonicalizable.\n";
        std::error_code error_code;
        std::filesystem::path current_working_directory{
         std::filesystem::current_path(error_code)};
        std::cerr << "II: " << current_working_directory << '\n';
        std::exit(EXIT_FAILURE);
    }
    extraction_script_path = *canonical_extraction_script_path_opt;

    std::string const shell_command = [&]{
        std::string result(extraction_script_path.string());
        result += ' ';
        result += material_balance;
        result += ' ';
        result += archive_path.string();
        return result;
    }();
    std::unique_ptr<FILE, decltype(&pclose)> pipe_ptr(
     popen(shell_command.c_str(), "r"), pclose);
    if (not pipe_ptr) {
        std::cerr << "EE: popen() failed for command.\nEE: " << shell_command
         << '\n';
        std::exit(EXIT_FAILURE);
    }

    return load_chesspresso_F_plb_impl<MAXINBITS, MAXOUTBITS>(
     [&pipe_ptr](char * buf, std::size_t nbytes) {
         fread(buf, nbytes, 1, pipe_ptr.get());
     },
     output_bit_count);
}


#endif // EGT_COMPARISON_LOAD_AND_SAVE_HPP
