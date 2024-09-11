#include "prober_type.hpp"


#include <iostream>

#include "probing.hpp"


// Try to copy the Syzygy data files to ${SCRATCH}/${TIMESTAMP} when
// these environment variables exist.  By using local, quick NVMe, we
// isolate subsequent Syzygy data accesses from network latency and
// contention from other machines using the mounted filesystem.
//
// TODO: Copying all the files makes sense when actually doing probe
// timings, but when exporting from Syzygy's format to
// probe_report_nybble, it's a shame to copy all the files over to
// then just export only one of them.
void try_to_speed_up_syzygy_disk_accesses() {
    char const * const env_SCRATCH{std::getenv("SCRATCH")};
    char const * const env_TIMESTAMP{std::getenv("TIMESTAMP")};
    if (nullptr != env_SCRATCH and nullptr != env_TIMESTAMP) {
        std::filesystem::path destination_dir{env_SCRATCH};
        destination_dir /= env_TIMESTAMP;
        std::filesystem::create_directories(destination_dir);
        char const * const env_EGT_DIR{std::getenv("EGT_DIR")};
        std::filesystem::path syzygy_top_dir{env_EGT_DIR};
        syzygy_top_dir /= "syzygy";
        for (unsigned int figurine_count = 3; figurine_count != 6;
         ++figurine_count) {
            std::filesystem::path const syzygy_figurine_count_dir{syzygy_top_dir
             / std::filesystem::path(std::to_string(figurine_count))};
            std::error_code error_code;
            std::filesystem::copy(syzygy_figurine_count_dir, destination_dir,
             error_code);
            if (std::error_code() != error_code) {
                std::cerr << "WW: Copy from " << syzygy_figurine_count_dir
                 << " to " << destination_dir << " resulted in " << error_code
                 << ".\n";
            }
        }
    }
}


// N.B. No concurrency protection is present: we assume only a single thread of
// execution exists.
std::optional<prober_type const>
prober_type::create_once_opt(std::string const & syzygy_path) {
    static bool syzygy_tb_init_successfully_called{false};
    static prober_type const prober;
    if (not syzygy_tb_init_successfully_called) {
        try_to_speed_up_syzygy_disk_accesses();
        unsigned int const figurine_count_available{probing_init(syzygy_path)};
        if (0 == figurine_count_available) {
            return std::nullopt;
        }
    }
    syzygy_tb_init_successfully_called = true;
    return prober;
}


probe_report_type prober_type::probe(tb_position_type const & tb_position) const {
    return egt_probe(tb_position);
}


probe_report_type prober_type::probe(std::string const & fen) const {
    return egt_probe(fen);
}


probe_report_type prober_type::probe(
    std::string const & fen,
    tb_position_type & tb_position
) const {
    return egt_probe(fen, tb_position);
}


probe_report_type prober_type::probe(
    std::string const & fen,
    tb_position_type & tb_position,
    unsigned int & tb_probe_root_result,
    std::array<unsigned int, TB_MAX_MOVES> & per_move_root_results
) const {
    return egt_probe(fen, tb_position, tb_probe_root_result,
     per_move_root_results);
}
