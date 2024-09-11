#include "chesspresso_endgame_table_type.hpp"


#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>

#include "representation/canonicalization_enum.hpp"
#include "representation/logic_digits_type.hpp"
#include "const_memory_stream_buffer_view_type.hpp"
#include "paths.hpp"
#include "unxz.hpp"


using representation::canonicalization_enum;
using representation::cover_enum;
using representation::cover_type;
using representation::cube_type;
using representation::egt_position_type;
using representation::logic_digits_type;
using representation::material_signature_type;


chesspresso_endgame_table_type::chesspresso_endgame_table_type(
    chesspresso_endgame_table_type const & other
) : endgame_table_type(canonicalization_enum::all_symmetries),
    the_header(other.the_header)
{
    for (std::size_t nth_cover{}; 3 != nth_cover; ++nth_cover) {
        representation::cover_type const & cover_to_copy{
         *other.the_FRD_cover_ptrs[nth_cover]};
        the_FRD_cover_ptrs[nth_cover] =
         std::make_unique<representation::cover_type>(cover_to_copy);
    }
}


chesspresso_endgame_table_type & chesspresso_endgame_table_type::operator=(
    chesspresso_endgame_table_type const & other
) {
    the_header = other.the_header;
    for (std::size_t nth_cover{}; 3 != nth_cover; ++nth_cover) {
        representation::cover_type const & cover_to_copy{
         *other.the_FRD_cover_ptrs[nth_cover]};
        the_FRD_cover_ptrs[nth_cover] =
         std::make_unique<representation::cover_type>(cover_to_copy);
    }
    return *this;
}


chesspresso_endgame_table_type::chesspresso_endgame_table_type()
  : endgame_table_type(canonicalization_enum::all_symmetries),
    the_header{0, 0, 0, 0},
    the_FRD_cover_ptrs{
     std::make_unique<representation::cover_type>(),
     std::make_unique<representation::cover_type>(),
     std::make_unique<representation::cover_type>()} {}


std::unique_ptr<chesspresso_endgame_table_type>
chesspresso_endgame_table_type::create_ptr(
    std::istream & in,
    std::size_t const covers_to_read
) {
    assert((1 <= covers_to_read) and (covers_to_read <= 3));
    auto table_ptr{std::make_unique<chesspresso_endgame_table_type>()};
    in.read(reinterpret_cast<char *>(table_ptr->the_header.data()),
     sizeof(table_ptr->the_header));
    if (not in.good()) {
        return nullptr;
    }
    std::uint64_t const per_cube_bit_count{table_ptr->the_header[0]};
    for (std::size_t nth_cover{}; nth_cover != covers_to_read; ++nth_cover) {
        std::size_t const cover_cube_count{
         table_ptr->the_header[1 + nth_cover]};
        table_ptr->the_FRD_cover_ptrs[nth_cover] =
         cover_type::create_ptr(in, cover_cube_count, per_cube_bit_count);
        if (nullptr == table_ptr->the_FRD_cover_ptrs[nth_cover]) {
            return nullptr;
        }
    }
    for (std::size_t nth_cover{3}; nth_cover != covers_to_read; --nth_cover) {
        // Mark any covers that we're not reading in as having size zero.
        table_ptr->the_header[nth_cover] = 0;
        table_ptr->the_FRD_cover_ptrs[nth_cover - 1] =
         std::make_unique<cover_type>();
    }

    // Ensure that we didn't encounter an error already.
    if (not in.good()) {
        return nullptr;
    }
    // Ensure that we're actually at the end of the stream if we tried to read
    // all three of the covers.
    if ((std::istream::traits_type::eof() != in.peek())
     and (3 == covers_to_read)) {
        return nullptr;
    }
    return table_ptr;
}


std::unique_ptr<chesspresso_endgame_table_type>
chesspresso_endgame_table_type::create_ptr(
    std::filesystem::path const & path,
    std::size_t const covers_to_read
) {
    std::ifstream data_in(path);
    return chesspresso_endgame_table_type::create_ptr(data_in, covers_to_read);
}


std::unique_ptr<chesspresso_endgame_table_type>
chesspresso_endgame_table_type::create_ptr(
    material_signature_type const & material_signature,
    std::size_t const covers_to_read
) {
    std::filesystem::path const path{chesspresso_path(std::nullopt,
     material_signature, std::nullopt, true, false)};
    return chesspresso_endgame_table_type::create_ptr(path, covers_to_read);
}


// The probe_report_type returned is to be interpreted from White's perspective.
probe_report_type chesspresso_endgame_table_type::probe_directly_impl(
    egt_position_type const & position
) const {
    assert(nullptr != the_FRD_cover_ptrs[0]);
    cover_type const & on_cover{*the_FRD_cover_ptrs[0]};
    logic_digits_type const input_as_pcn{position.as_logic_digits()};
    std::size_t input_bit_count{2 * input_as_pcn.size()};
    std::size_t bit_count{3 + input_bit_count};
    cube_type probe_cube(prepare_cube_for_cover(input_as_pcn,
     cover_enum::function, redundant));

    std::vector<cube_type>::const_iterator const found{
     on_cover.find_containing_cube(probe_cube)};
    if (found == on_cover.cend()) {
        return unknown;
    }

    std::bitset<3> white_result;
    white_result[0] = (*found)[bit_count - 1];
    white_result[1] = (*found)[bit_count - 2];
    white_result[2] = (*found)[bit_count - 3];

    // N.B.  The specified bitset binary order for 0bxxx is wlos, wdrw, wwin!
    if (std::bitset<3>{0b010} == white_result) {
        return draw;
    } else if (std::bitset<3>{0b001} == white_result) {
        return win;
    } else if (std::bitset<3>{0b100} == white_result) {
        return loss;
    } else if (std::bitset<3>{0b011} == white_result) {
        return cursed_win;
    } else if (std::bitset<3>{0b110} == white_result) {
        return blessed_loss;
    }
    return unknown;
}


std::unique_ptr<chesspresso_endgame_table_type>
read_chesspresso_endgame_table(
    std::filesystem::path const & input_path,
    std::size_t const covers_to_read
) {
    assert((1 <= covers_to_read) and (covers_to_read <= 3));
    auto const uncompressed_data_opt(
     vector_with_uncompressed_data_opt(input_path));
    if (not uncompressed_data_opt.has_value()) {
        return nullptr;
    }
    const_memory_stream_buffer_view_type uncompressed_data(
     uncompressed_data_opt->data(),
     uncompressed_data_opt->data() + uncompressed_data_opt->size());
    std::istream uncompressed_data_in(&uncompressed_data);
    return chesspresso_endgame_table_type::create_ptr(uncompressed_data_in,
     covers_to_read);
}


void write_cover_as_is(
    std::ostream & out,
    cover_type const & cover,
    std::streamsize const bytes_per_cube
) {
    std::size_t const cube_count{cover.size()};
    for (std::size_t nth_cube{}; nth_cube != cube_count; ++nth_cube) {
        out.write(reinterpret_cast<char const *>(
         cover[nth_cube].uint64s().data()), bytes_per_cube);
    }
}


void write_sorted_cover(
    std::ostream & out,
    cover_type const & cover,
    std::streamsize const bytes_per_cube
) {
    if (std::is_sorted(cover.begin(), cover.end())) {
        write_cover_as_is(out, cover, bytes_per_cube);
        return;
    }
    cover_type sorted{cover};
    std::sort(sorted.begin(), sorted.end());
    write_cover_as_is(out, sorted, bytes_per_cube);
}


bool chesspresso_endgame_table_type::write(
    std::ostream & out,
    bool const ensure_ordered
) const {
    out.write(reinterpret_cast<char const *>(the_header.data()),
     sizeof(the_header));
    for (std::size_t nth_cover{}; nth_cover != 3; ++nth_cover) {
        if (ensure_ordered) {
            write_sorted_cover(out, *the_FRD_cover_ptrs[nth_cover],
             bytes_per_cube());
        } else {
            write_cover_as_is(out, *the_FRD_cover_ptrs[nth_cover],
             bytes_per_cube());
        }
    }
    return not out.good();
}


void print_cover(
    std::ostream & out,
    bit_region_schema_type const & schema,
    cover_type const & cover,
    std::string_view const output_map
) {
    for (cube_type const & cube: cover) {
        cube.print(out, schema, output_map);
        out.put('\n');
    }
}


bool chesspresso_endgame_table_type::print(
    std::ostream & out,
    std::size_t const covers_to_print
) const {
    assert((1 <= covers_to_print) and (covers_to_print <= 3));
    std::uint64_t const per_cube_bit_count{the_header[0]};
    assert(3 < per_cube_bit_count);
    std::uint64_t const per_cube_output_bit_count{3};
    std::uint64_t const per_cube_input_bit_count{
     per_cube_bit_count - per_cube_output_bit_count};
    assert(0 == (0b1 & per_cube_input_bit_count));
    std::uint64_t const per_cube_input_bit_pair_count{
     per_cube_input_bit_count >> 1};
    assert(1 == per_cube_input_bit_pair_count % 6);
    [[maybe_unused]] std::uint64_t const figurine_count{
     per_cube_input_bit_pair_count / 6};

    auto schema_ptr{bit_region_schema_type::create_ptr(
     per_cube_input_bit_pair_count, std::vector<std::size_t>{},
     per_cube_output_bit_count)};
    switch (covers_to_print) {
    case 1:
        out << ".type f\n";
        break;
    case 2:
        out << ".type fr\n";
        break;
    case 3:
        out << ".type fdr\n";
        break;
    default:
        assert(false);
        __builtin_unreachable();
    }
    std::size_t cube_count_to_print{the_header[1]};
    if (2 <= covers_to_print) {
        cube_count_to_print += the_header[2];
    }
    if (3 == covers_to_print) {
        cube_count_to_print += the_header[3];
    }
    out << ".i " << (per_cube_input_bit_pair_count) << "\n.o "
     << per_cube_output_bit_count << "\n.p " << cube_count_to_print << '\n';
    print_cover(out, *schema_ptr, cover(cover_enum::function), "~1");
    if (2 <= covers_to_print) {
        print_cover(out, *schema_ptr, cover(cover_enum::reverse), "~0");
    }
    if (3 == covers_to_print) {
        print_cover(out, *schema_ptr, cover(cover_enum::do_not_care), "~2");
    }
    out << ".end" << std::endl;
    return not out.good();
}


bool write_chesspresso_endgame_table(
    chesspresso_endgame_table_type const & table,
    std::filesystem::path const & output_path,
    bool const ensure_ordered
) {
    std::ofstream out(output_path, std::ios_base::binary | std::ios_base::out);
    return table.write(out, ensure_ordered);
}
