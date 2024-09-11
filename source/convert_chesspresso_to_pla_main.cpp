#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>


#include "chesspresso_endgame_table_type.hpp"


void dump_usage() {
    std::cerr << "EE: The first argument specifies the covers to convert.\n"
     "EE: (1 = F only, 2 = F and R, 3 = F, R, and D)\n"
     "EE: Any subsequent arguments each specify a chesspresso input file.\n"
     "EE: (Each file must end in .plb.xz.)\n";
}


// The first argument is how many covers to convert (1 = F, 2 = FR, 3 = FRD).
// The second argument is the path to the chesspresso file being converted.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        dump_usage();
        return EXIT_FAILURE;
    }

    std::size_t covers_to_convert{std::stoull(std::string(argv[1]))};
    if ((covers_to_convert < 1) or (3 < covers_to_convert)) {
        dump_usage();
        return EXIT_FAILURE;
    }

    for (int nth_file{1}; nth_file + 1 != argc; ++nth_file) {
        std::filesystem::path const input_path{argv[1 + nth_file]};
        std::filesystem::path output_path{input_path};
        if (".xz" != output_path.extension()) {
            std::cerr << "EE: Expected only .plb.xz files to be specified.\n";
            return EXIT_FAILURE;
        }
        output_path.replace_extension();
        if (".plb" != output_path.extension()) {
            std::cerr << "EE: Expected only .plb.xz files to be specified.\n";
            return EXIT_FAILURE;
        }
        output_path.replace_extension(".f");
        if (2 <= covers_to_convert) {
            if (3 == covers_to_convert) {
                output_path += "d";
            }
            output_path += "r";
        }
        output_path += ".pla";

        std::unique_ptr<chesspresso_endgame_table_type> table_ptr{
         read_chesspresso_endgame_table(input_path, covers_to_convert)};
        if (not table_ptr) {
            std::cerr << "EE: Unable to interpret " << input_path << ".\n";
            continue;
        }
        std::ofstream out{output_path, std::ios_base::out};
        table_ptr->print(out, covers_to_convert);
        out.close();
        if (out.fail()) {
            std::cerr << "EE: Error while printing " << output_path << ".\n";
        }
    }
}
