#include "syzygy_position_processor_main.hpp"


#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "representation/canonicalization_enum.hpp"
#include "representation/perspective_enum.hpp"
#include "argument_parsing.hpp"
#include "syzygy_position_processor_enum.hpp"
#include "syzygy_position_processor_type.hpp"
#include "syzygy_position_processor_factory_type.hpp"


using representation::material_signature_type;


void describe_usage(char const * executable_name) {
    std::clog << executable_name << " --processor=[emitter] "
     "--canonicalization=[canonicalized|noncanonicalized] "
     "--perspective=[side_to_move|white_player] "
     "--postprocess=[true|false] "
     "material_signature [material_signature...]\n"
     "    where emitter is one of {debugger, probe_report_nybble_emitter}."
     << std::endl;
}


std::tuple<
    syzygy_position_processor_enum,
    representation::canonicalization_enum,
    representation::perspective_enum,
    bool
> parse_mandatory_arguments(int argc, char *argv[]) {
    if (argc < 4) {
        describe_usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
    std::string const processor_argument{remove_potential_prefix(argv[1],
     "--processor=")};
    std::string const canonicalization_argument{remove_potential_prefix(argv[2],
     "--canonicalization=")};
    std::string const perspective_argument{remove_potential_prefix(argv[3],
     "--perspective=")};
    std::string const postprocess_argument{remove_potential_prefix(argv[4],
     "--postprocess=")};

    std::optional<syzygy_position_processor_enum> const processor_opt{
     as_syzygy_position_processor_enum_opt(processor_argument)};
    if (not processor_opt) {
        std::cerr << "EE: --processor=" << processor_argument
         << " was not understood.\n";
    }
    std::optional<representation::canonicalization_enum> const
     canonicalization_opt{representation::as_canonicalization_enum_opt(
     canonicalization_argument)};
    if (not canonicalization_opt) {
        std::cerr << "EE: --canonicalization=" << canonicalization_argument
         << " was not understood.\n";
    }
    std::optional<representation::perspective_enum> const perspective_opt{
     representation::as_perspective_enum_opt(perspective_argument)};
    if (not perspective_opt) {
        std::cerr << "EE: --perspective=" << perspective_argument
         << " was not understood.\n";
    }
    std::optional<bool> const postprocess_opt{
     as_bool_opt(postprocess_argument)};
    if (not postprocess_opt) {
        std::cerr << "EE: --postprocess=" << postprocess_argument
         << " was not understood.\n";
    }

    if (not (processor_opt and canonicalization_opt and perspective_opt
     and postprocess_opt)) {
        describe_usage(argv[0]);
        std::exit(EXIT_FAILURE);
    }
    return std::make_tuple(*processor_opt, *canonicalization_opt,
     *perspective_opt, *postprocess_opt);
}


int main(int argc, char * argv[]) {
    auto [processor, canonicalization, perspective, also_do_postprocessing]{
     parse_mandatory_arguments(argc, argv)};
    std::vector<material_signature_type> const material_signatures{
     material_signatures_to_process(argc - 4, argv + 4)};
    syzygy_position_processor_factory_type factory;
    std::unique_ptr<syzygy_position_processor_type> const processor_ptr{
     factory.create_ptr(processor)};
    processor_ptr->process_tables(canonicalization, perspective,
     material_signatures, also_do_postprocessing);
}
