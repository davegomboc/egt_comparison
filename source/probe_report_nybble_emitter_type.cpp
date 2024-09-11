#include "probe_report_nybble_emitter_type.hpp"


#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>


using representation::canonicalization_enum;
using representation::egt_position_type;
using representation::figurine_enum;
using representation::material_signature_type;
using representation::perspective_enum;


void probe_report_nybble_emitter_type::on_process_table_commencement(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature
) {
    std::error_code const create_dirs_result{ensure_output_parent_path_exists(
     perspective, material_signature)};
    if (std::error_code{} != create_dirs_result) {
        std::cerr << "EE: Unable to create parent directories.\n";
        std::exit(EXIT_FAILURE);
    }
    std::filesystem::path path(probe_report_nybble_path(canonicalization, perspective,
     material_signature, false));
    the_output_stream_ptr = std::make_unique<std::ofstream>(path);
    if (not the_output_stream_ptr->good()) {
        std::cerr << "EE: Unable to establish " << path << ".\n";
        std::exit(EXIT_FAILURE);
    }
    std::cerr << "II: Established " << path << ".\n";
    the_file_path = path;

    // Within each byte, we store both black to move (in the high nybble) and
    // white to move (in the low nybble).
    the_probe_reports = std::vector<std::byte>(
     1ull << (6 * material_signature.size()), std::byte{});
}


void probe_report_nybble_emitter_type::on_process_table_completion(
    canonicalization_enum const canonicalization,
    perspective_enum const perspective,
    material_signature_type const & material_signature
) {
    dump_probe_report_counters(std::clog);

    assert(the_probe_reports.size()
     <= std::numeric_limits<std::streamsize>::max());
    std::streamsize const chars_to_write_count{
     static_cast<std::streamsize>(the_probe_reports.size())};
    the_output_stream_ptr->write(
     reinterpret_cast<char const *>(the_probe_reports.data()),
     chars_to_write_count);
    the_output_stream_ptr->flush();

    std::vector<std::pair<std::string, std::string>> more_metadata{};
    more_metadata.push_back(std::make_pair<std::string, std::string>(
     "tellp_at_completion", std::to_string(the_output_stream_ptr->tellp())));
    record_additional_metadata(more_metadata);

    if (not the_output_stream_ptr->good()) {
        std::cerr << "EE: Writing the probe nybbles for " << material_signature
         << " (" << as_string_view(canonicalization) << ", "
         << as_string_view(perspective) << ") failed.\n";
        std::exit(EXIT_FAILURE);
    }
    the_output_stream_ptr->close();
    the_output_stream_ptr.reset();
}


void probe_report_nybble_emitter_type::on_postprocess_table(
    canonicalization_enum const /*canonicalization*/,
    perspective_enum const /*perspective*/,
    material_signature_type const & /*material_signature*/
) {
    std::string compress{"xz -9 -f "};
    compress += the_file_path.string();
    std::system(compress.c_str());
    std::string chmod{"chmod 440 "};
    chmod += the_file_path.string();
    chmod += ".xz";
    std::system(chmod.c_str());
    the_file_path.clear();
}


/*
                  /---------finalized_probe_report---------\
    probe_report  wtm_nybble  btm_spr_nybble  btm_wpr_nybble
    ============  ==========  ==============  ==============
         unknown        0000            0000            0000
            loss        0001            0001            0101
    blessed_loss        0010            0010            0100
            draw        0011            0011            0011
      cursed_win        0100            0100            0010
             win        0101            0101            0001
         invalid        0110            0110            0110
         illegal        0111            0111            0111
       redundant        1000            1000            1000
*/
void probe_report_nybble_emitter_type::record(
    material_signature_type const & material_signature,
    std::size_t const position_id,
    egt_position_type const & /*noncanonicalized_egt_position*/,
    probe_report_type const finalized_probe_report
) {
    std::size_t const used_bit_count{1 + 6 * material_signature.size()};
    std::size_t const used_bit_mask{(UINT64_C(1) << used_bit_count) - 1};
    // No bits numbered higher than the side to move bit should ever be set.
    assert(position_id == (used_bit_mask & position_id));

    // We avoid assuming that the position ids are being processed in increasing
    // order (0, 1, 2, ...) by storing all the values into memory, using the low
    // nybble of each byte for white to move, and the high nybble of each byte
    // for black to move.
    std::size_t const black_to_move_bit_mask{
     used_bit_mask ^ (used_bit_mask >> 1)};
    std::size_t const byte_index{position_id & ~(black_to_move_bit_mask)};
    bool const black_to_move{0 != (position_id & black_to_move_bit_mask)};
    std::byte to_store{finalized_probe_report.as_index()};
    if (black_to_move) {
        assert(std::byte{}
         == (the_probe_reports[byte_index] & std::byte{0b11110000}));
        the_probe_reports[byte_index] |= to_store << 4;
    } else {
        assert(std::byte{}
         == (the_probe_reports[byte_index] & std::byte{0b00001111}));
        the_probe_reports[byte_index] |= to_store;
    }
}


// This primarily exists so that clang++ will emit vtables for this class in
// this translation unit only, instead of all of them.  (See -Wweak-vtables
// documentation for details.)
void probe_report_nybble_emitter_type::vtable_pinning_method() const {}
