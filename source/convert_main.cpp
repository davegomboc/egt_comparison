#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "cube_list.hpp"
#include "data_category_enum.hpp"
#include "decision_dag.hpp"
#include "indexed_cube_list.hpp"
#include "load_and_save.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using ddag_type = ddag<32, 8, 27>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


template <
    std::size_t const MAXINBITS,
    std::size_t const MAXOUTBITS,
    std::size_t const NINDEXBITS,
    std::size_t const NTREEINDEXBITS
> void save(
    indexed_cube_list_tt<
        cube_list_tt<MAXINBITS, MAXOUTBITS>,
        NINDEXBITS,
        NTREEINDEXBITS
    > const & icl,
    std::ostream & data_out,
    data_category_enum const save_category
) {
    if (data_category_enum::indexed_cube_list == save_category) {
        bool const write_failed{icl.serialize(data_out)};
        if (write_failed) {
            std::exit(EXIT_FAILURE);
        }
    } else {
        cube_list_tt<MAXINBITS, MAXOUTBITS> cl{icl.cubes()};
        save(cl, data_out, save_category);
    }
}

template <
    std::size_t const MAXINBITS,
    std::size_t const MAXOUTBITS
> void save(
    cube_list_tt<MAXINBITS, MAXOUTBITS> & cl,
    std::ostream & data_out,
    data_category_enum const save_category
) {
    cl.ensure_sorted();
    std::cerr << "II: cl.nbytes() = " << cl.nbytes() << '\n';
    if (data_category_enum::cube_list == save_category) {
        bool const write_failed{cl.serialize(data_out)};
        if (write_failed) {
            std::exit(EXIT_FAILURE);
        }
    } else if (data_category_enum::ddag == save_category) {
        ddag_type const dt(cl);
        std::cerr << "II: dt.nbytes() = " << dt.nbytes() << '\n';
        save_ddag(data_out, dt);
    } else if (data_category_enum::indexed_cube_list == save_category) {
        indexed_cube_list_type const icl(cl);
        std::cerr << "II: icl.nbytes() = " << icl.nbytes() << '\n';
        bool const write_failed{icl.serialize(data_out)};
        if (write_failed) {
            std::exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "EE: Saving as " << extension(save_category)
         << " is not yet implemented.\n";
        std::exit(EXIT_FAILURE);
    }
}

void save(
    ddag_type const & dt,
    std::ostream & data_out,
    data_category_enum const save_category
) {
    std::cerr << "II: dt.nbytes() = " << dt.nbytes() << '\n';
    if (data_category_enum::ddag == save_category) {
        save_ddag(data_out, dt);
    } else {
        cube_list_type cl(dt.to_cube_list());
        cl.ensure_sorted();
        if (data_category_enum::cube_list == save_category) {
            std::cerr << "II: cl.nbytes() = " << cl.nbytes() << '\n';
            bool const write_failed{cl.serialize(data_out)};
            if (write_failed) {
                std::exit(EXIT_FAILURE);
            }
        } else if (data_category_enum::indexed_cube_list == save_category) {
            indexed_cube_list_type const icl(cl);
            std::cerr << "II: icl.nbytes() = " << icl.nbytes() << '\n';
            bool const write_failed{icl.serialize(data_out)};
            if (write_failed) {
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "EE: Saving as " << extension(save_category)
             << " is not yet implemented.\n";
            std::exit(EXIT_FAILURE);
        }
    }
}

void convert(
    std::string const & load_spec,
    std::optional<data_category_enum> load_category_opt,
    std::string const & save_spec,
    data_category_enum save_category
) {
    std::ofstream file_out;
    bool const stdout_requested{'-' == save_spec[0]};
    std::ostream & data_out(stdout_requested ? std::cout
     : file_out = open_file_write(save_spec));
    if (load_category_opt) {
        std::ifstream file_in;
        bool const stdin_requested{'-' == load_spec[0]};
        std::istream & data_in(stdin_requested ? std::cin
         : file_in = open_file_read(load_spec));
        if (data_category_enum::chesspresso_binary == *load_category_opt) {
            cube_list_type cl{load_chesspresso_F_plb<32, 8>(data_in)};
            save(cl, data_out, save_category);
        } else if (data_category_enum::cube_list == *load_category_opt) {
            std::optional<cube_list_type> cube_list_opt{
             cube_list_type::create_opt(data_in)};
            if (not cube_list_opt) {
                std::cerr << "EE: Load failure.\n";
                std::exit(EXIT_FAILURE);
            }
            save(*cube_list_opt, data_out, save_category);
        } else if (data_category_enum::indexed_cube_list
         == *load_category_opt) {
            std::optional<indexed_cube_list_type> const indexed_cube_list_opt{
             indexed_cube_list_type::create_opt(data_in)};
            if (not indexed_cube_list_opt) {
                std::cerr << "EE: Load failure.\n";
                std::exit(EXIT_FAILURE);
            }
            save(*indexed_cube_list_opt, data_out, save_category);
        } else if (data_category_enum::ddag == *load_category_opt) {
            save(load_ddag<32, 8, 27>(data_in), data_out, save_category);
        } else {
            std::cerr << "EE: Loading as " << extension(save_category)
             << " is not yet implemented.\n";
            std::exit(EXIT_FAILURE);
        }
    } else {
        std::string material_balance(load_spec.substr(4));
        cube_list_type cl{
         load_chesspresso_F_plb_from_archive<32, 8>(material_balance, 3)};
        save(cl, data_out, save_category);
    }
    if (not stdout_requested) {
        (void)update_permissions(save_spec,
         std::filesystem::perms::owner_read,
         std::filesystem::perm_options::replace);
    }
}

[[noreturn]] void print_usage(char const * const executable_name) {
    std::cerr << "usage: " << executable_name
     << " <load_spec> <save_spec>\n"
     "where load_spec may be either a path, or -<extension> to use stdin, or "
     "tar:<material_balance>\n"
     "(which accesses <material_balance>.plb.xz within a tar file)\n"
     "where save_spec can be either a path, or -<extension> to use stdout\n"
     "valid extensions are:\n"
     "   .clst: cube list\n"
     "   .dtr:  decision directed acyclic graph\n"
     "   .icl:  indexed cube list [saving only]\n"
     "   .plb:  chesspresso binary format (reads F) [loading only]\n";
    std::exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {
    if (argc < 3) {
        print_usage(argv[0]);
    }
    std::string load_spec{argv[1]};
    std::string save_spec{argv[2]};
    if (load_spec.empty() or save_spec.empty()) {
        print_usage(argv[0]);
    }
    std::optional<data_category_enum> load_category_opt;
    if ("tar:" != load_spec.substr(0, 4)) {
        load_category_opt = categorize_by_extension_opt(load_spec);
        if (not load_category_opt) {
            print_usage(argv[0]);
        }
    }
    std::optional<data_category_enum> save_category_opt{
     categorize_by_extension_opt(save_spec)};
    if (not save_category_opt
     or (data_category_enum::chesspresso_binary == *save_category_opt)) {
        print_usage(argv[0]);
    }
    convert(argv[1], load_category_opt, argv[2], *save_category_opt);
}
