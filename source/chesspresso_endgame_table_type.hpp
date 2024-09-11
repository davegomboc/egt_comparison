#if !defined(EGT_COMPARISON_CHESSPRESSO_ENDGAME_TABLE_TYPE_HPP)
#define EGT_COMPARISON_CHESSPRESSO_ENDGAME_TABLE_TYPE_HPP


#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

#include "representation/cube_type.hpp"
#include "representation/cover_enum.hpp"
#include "representation/cover_type.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "bit_indexing_and_sizing.hpp"
#include "default_init_type.hpp"
#include "endgame_table_type.hpp"
#include "probe_report_type.hpp"


class chesspresso_endgame_table_type final : public endgame_table_type {

public:
    chesspresso_endgame_table_type(chesspresso_endgame_table_type &&) = default;
    chesspresso_endgame_table_type & operator=(
     chesspresso_endgame_table_type &&) = default;

    chesspresso_endgame_table_type(chesspresso_endgame_table_type const & other);
    chesspresso_endgame_table_type & operator=(
     chesspresso_endgame_table_type const & other);

    virtual ~chesspresso_endgame_table_type() override = default;
    chesspresso_endgame_table_type();

    // covers_to_read may be:
    // 1: read in F, but neither R nor D.
    // 2: read in F and R, but not D.
    // 3: read in F, R, and D.
    static std::unique_ptr<chesspresso_endgame_table_type> create_ptr(
        std::istream & in,
        std::size_t covers_to_read = 3);
    static std::unique_ptr<chesspresso_endgame_table_type> create_ptr(
        std::filesystem::path const & path,
        std::size_t covers_to_read = 3);
    static std::unique_ptr<chesspresso_endgame_table_type> create_ptr(
        representation::material_signature_type const & material_signature,
        std::size_t covers_to_read = 3);

    representation::cover_type const & cover(
        representation::cover_enum cover
    ) const {
        return *the_FRD_cover_ptrs[static_cast<std::size_t>(cover)];
    }

    virtual std::filesystem::path extension() const override {
        return ".plb";
    }

    probe_report_type probe_directly_impl(
        representation::egt_position_type const & position) const override;

    // Returns false iff successful.  The output will be the text .pla.
    bool print(std::ostream & out, std::size_t covers_to_print) const;
    // Returns false iff successful.  The output will be the binary format.
    bool write(std::ostream & out, bool ensure_ordered) const;

private:
    // Requires the_header[0] to be properly initialized.
    std::uint64_t uint64_ts_per_cube() const {
        return required<std::uint64_t>(the_header[0]);
    }
    // Requires the_header[0] to be properly initialized.
    std::streamsize bytes_per_cube() const {
        return static_cast<std::streamsize>(
         sizeof(std::uint64_t) * uint64_ts_per_cube());
    }

    friend class epp_chesspresso_endgame_table_generator_type;
    friend class chesspresso_endgame_table_generator_type;

    // per_cube_bit_count, F_cube_count, R_cube_count, D_cube_count.
    std::array<std::uint64_t, 4> the_header;

    std::array<std::unique_ptr<representation::cover_type>, 3>
     the_FRD_cover_ptrs;
};


// covers_to_read may be:
// 1: read in F, but neither R nor D.
// 2: read in F and R, but not D.
// 3: read in F, R, and D.
std::unique_ptr<chesspresso_endgame_table_type> read_chesspresso_endgame_table(
    std::filesystem::path const & input_path,
    std::size_t covers_to_read);

bool write_chesspresso_endgame_table(
    chesspresso_endgame_table_type const & table,
    std::filesystem::path const & output_path,
    bool ensure_ordered);


#endif // EGT_COMPARISON_CHESSPRESSO_ENDGAME_TABLE_TYPE_HPP
