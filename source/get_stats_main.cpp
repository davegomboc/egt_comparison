#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <variant>

#include "cube_list.hpp"
#include "data_category_enum.hpp"
#include "decision_dag.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using ddag_type = ddag<32, 8, 27>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;
using variant_type = std::variant<std::monostate, cube_list_type, ddag_type,
 indexed_cube_list_type>;


// Returns true iff an error occurred.
bool dump_stats(variant_type const & object) {
    if (std::holds_alternative<ddag_type>(object)) {
        ddag_type const & value{std::get<ddag_type>(object)};
        std::cerr << "II: nbytes() = " << value.nbytes() << "\n"
         "II: ncubes() = " << value.ncubes() << "\n"
         "II: nnodes() = " << value.nnodes() << "\n";
        return false;
    }

    if (std::holds_alternative<cube_list_type>(object)) {
        cube_list_type const & value{std::get<cube_list_type>(object)};
        std::cerr << "II: nbytes() = " << value.nbytes() << "\n"
         "II: ncubes() = " << value.ncubes() << "\n";
        return false;
    }

    if (std::holds_alternative<indexed_cube_list_type>(object)) {
        indexed_cube_list_type const & value{
         std::get<indexed_cube_list_type>(object)};
        std::cerr << "II: nbytes() = " << value.nbytes() << "\n"
         "II: ncubes() = " << value.ncubes() << "\n";
        return false;
    }

    std::cerr << "EE: Do not know how to dump stats for this object.\n";
    return true;
}


variant_type load(
    std::string const & load_spec,
    std::optional<data_category_enum> load_category_opt
) {
    if ("tar:" == load_spec.substr(0, 4)) {
        std::string const material_balance{load_spec.substr(4)};
        return load_chesspresso_F_plb_from_archive<32, 8>(material_balance);
    }
    bool const stdin_requested{'-' == load_spec[0]};
    std::ifstream file_in;
    std::istream & data_in{stdin_requested ? std::cin
     : file_in = open_file_read(load_spec)};
    if (data_category_enum::chesspresso_binary == *load_category_opt) {
        return load_chesspresso_F_plb<32, 8>(data_in);
    }
    if (data_category_enum::ddag == *load_category_opt) {
        return load_ddag<32, 8, 27>(data_in);
    }
    if (data_category_enum::cube_list == *load_category_opt) {
        auto const cube_list_opt{cube_list_type::create_opt(data_in)};
        if (not cube_list_opt) {
            std::exit(EXIT_FAILURE);
        }
        return *cube_list_opt;
    }
    if (data_category_enum::indexed_cube_list == *load_category_opt) {
        auto const indexed_cube_list_opt{
         indexed_cube_list_type::create_opt(data_in)};
        if (not indexed_cube_list_opt) {
            std::exit(EXIT_FAILURE);
        }
        return *indexed_cube_list_opt;
    }
    std::cerr << "EE: Unhandled input file type.\n";
    return variant_type{};
}


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name << " <load_spec>\n"
     "where load_spec may be either a path, or -<extension> to use stdin, or "
     "tar:<material_balance>\n"
     "(which accesses <material_balance>.plb.xz within a tar file)\n"
     "valid extensions are:\n"
     "   .clst: cube list\n"
     "   .dtr:  decision directed acyclic graph\n"
     "   .icl:  indexed cube list\n"
     "   .plb:  chesspresso binary format (reads F)\n";
    std::exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {
    if (2 != argc) {
        print_usage(argv[0]);
    }
    std::string const load_spec(argv[1]);
    if (load_spec.empty()) {
        print_usage(argv[0]);
    }
    std::optional<data_category_enum> load_category_opt;
    if ("tar:" != load_spec.substr(0, 4)) {
        load_category_opt = categorize_by_extension_opt(load_spec);
        if (not load_category_opt) {
            print_usage(argv[0]);
        }
    }
    variant_type const object{load(argv[1], load_category_opt)};

    bool const error_occurred{dump_stats(object)};
    if (error_occurred) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
