#if !defined(EGT_COMPARISON_INDEXED_CUBE_LIST_ENDGAME_TABLE_TYPE_HPP)
#define EGT_COMPARISON_INDEXED_CUBE_LIST_ENDGAME_TABLE_TYPE_HPP


#include <filesystem>
#include <memory>
#include <string_view>

#include "representation/egt_position_type.hpp"
#include "representation/material_signature_type.hpp"
#include "board_generation_helpers.hpp"
#include "cube_list.hpp"
#include "endgame_table_type.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"
#include "probe_report_type.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;

class indexed_cube_list_endgame_table_type final : public endgame_table_type {

public:
    indexed_cube_list_endgame_table_type & operator=(
     indexed_cube_list_endgame_table_type const &) = delete;
    indexed_cube_list_endgame_table_type & operator=(
     indexed_cube_list_endgame_table_type &&) = default;
    indexed_cube_list_endgame_table_type(
     indexed_cube_list_endgame_table_type const &) = delete;
    indexed_cube_list_endgame_table_type(
     indexed_cube_list_endgame_table_type &&) = default;
    virtual ~indexed_cube_list_endgame_table_type() override = default;
    indexed_cube_list_endgame_table_type(indexed_cube_list_type && cubes);

    static std::unique_ptr<indexed_cube_list_endgame_table_type> create_ptr(
     std::istream & in);
    static std::unique_ptr<indexed_cube_list_endgame_table_type> create_ptr(
     std::filesystem::path const & path);
    static std::unique_ptr<indexed_cube_list_endgame_table_type> create_ptr(
     representation::material_signature_type const & material_signature,
     std::string_view const proc);
    static std::unique_ptr<indexed_cube_list_endgame_table_type> create_ptr(
     representation::material_signature_type const & material_signature);

    virtual std::filesystem::path extension() const override {
        return ".icl";
    }

    probe_report_type probe_directly_impl(
        representation::egt_position_type const & position) const override;

private:
    indexed_cube_list_type the_endgame_data;
};


#endif // EGT_COMPARISON_INDEXED_CUBE_LIST_ENDGAME_TABLE_TYPE_HPP
