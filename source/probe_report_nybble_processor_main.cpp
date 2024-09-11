#include "probe_report_nybble_processor_main.hpp"


#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "representation/canonicalization_enum.hpp"
#include "representation/material_signature_type.hpp"
#include "representation/perspective_enum.hpp"

#include "argument_parsing.hpp"
#include "probe_report_nybble_processor_enum.hpp"
#include "probe_report_nybble_processor_factory_type.hpp"
#include "probe_report_nybble_processor_type.hpp"


using representation::canonicalization_enum;
using representation::material_signature_type;
using representation::perspective_enum;


void describe_usage(char const * const executable_name) {
    std::cerr << executable_name << " --processor=[emitter] "
     "--canonicalization=[all_symmetries|single_symmetry] "
     "--perspective=[side_to_move|white_player] "
     "--cover_count=[1|2|3] "
     "material_signature [material_signature...]\n"
     "    where emitter is one of {debugger, "
     "chesspresso_endgame_table_generator}.\n";
}


std::tuple<
    probe_report_nybble_processor_enum,
    canonicalization_enum,
    perspective_enum,
    unsigned int
> parse_mandatory_arguments(int argc, char * argv[]) {
    if (argc < 5) {
        describe_usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
    std::string const processor_argument{remove_potential_prefix(argv[1],
     "--processor=")};
    std::string const canonicalization_argument{remove_potential_prefix(argv[2],
     "--canonicalization=")};
    std::string const perspective_argument{remove_potential_prefix(argv[3],
     "--perspective=")};

    std::optional<probe_report_nybble_processor_enum> const processor_opt{
     as_probe_report_nybble_processor_enum_opt(processor_argument)};
    if (not processor_opt) {
        std::cerr << "EE: --processor was not undertstood.\n";
    }
    std::optional<canonicalization_enum> const canonicalization_opt{
     representation::as_canonicalization_enum_opt(canonicalization_argument)};
    if (not canonicalization_opt) {
        std::cerr << "EE: --canonicalization was not understood.\n";
    }
    std::optional<perspective_enum> const perspective_opt{
     representation::as_perspective_enum_opt(perspective_argument)};
    if (not perspective_opt) {
        std::cerr << "EE: --perspective was not understood.\n";
    }

    if (not (processor_opt and canonicalization_opt and perspective_opt)) {
        describe_usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }

    std::string const cover_count_argument{remove_potential_prefix(argv[4],
     "--cover_count=")};
    int cover_count_to_generate{std::atoi(cover_count_argument.c_str())};
    if ((cover_count_to_generate < 1) or (3 < cover_count_to_generate)) {
        std::cerr << "EE: Specify 1 to generate F, 2 to generate F and R, or "
         "3 to generate F, R, and D.\n";
        describe_usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
    return std::make_tuple(*processor_opt, *canonicalization_opt,
     *perspective_opt, static_cast<unsigned int>(cover_count_to_generate));
}


int main(int argc, char *argv[]) {
    auto const [processor, canonicalization, perspective,
     cover_count_to_generate]{parse_mandatory_arguments(argc, argv)};
    std::vector<material_signature_type> const material_signatures{
     material_signatures_to_process(argc - 4, argv + 4)};

    probe_report_nybble_processor_factory_type factory;
    for (material_signature_type const & signature: material_signatures) {
        std::unique_ptr<probe_report_nybble_processor_type> const
         processor_ptr{factory.create_ptr(processor, canonicalization,
         perspective, signature, cover_count_to_generate)};
        processor_ptr->process();
    }
}
