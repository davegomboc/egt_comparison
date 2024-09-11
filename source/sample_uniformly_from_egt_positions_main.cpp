#include <iomanip>
#include <iostream>

#include "epd/extended_position_description_type.hpp"

#include "probe_report_type.hpp"

#include "position_sampling_type.hpp"


[[noreturn]] void print_usage(char const * const executable_name) {
    std::cout << "usage: " << executable_name
     << " <figurine_count (2-5)> <position_count (non-negative)>" << std::endl;
    std::exit(EXIT_FAILURE);
}


int main(int argc, char * argv[]) {
    if (3 != argc) {
        print_usage(argv[0]);
    }
    int figurine_count{std::atoi(argv[1])};
    if ((figurine_count < 2) or (5 < figurine_count)) {
        print_usage(argv[0]);
    }
    long long position_count{std::atoll(argv[2])};
    if (position_count < 0) {
        print_usage(argv[0]);
    }
    position_sampling_type sampler;
    for (std::size_t i{}; i != static_cast<std::size_t>(position_count); ++i) {
        auto [probe_report, position]{
         sampler.draw_egt_entrance_position_uniformly(
         static_cast<unsigned int>(figurine_count))};
        std::optional<epd::extended_position_description_type>
         extended_position_description_opt(
         position.as_extended_position_description_opt());
        assert(extended_position_description_opt);
        extended_position_description_opt->operations()
         = sampler.generate_epd_operations(probe_report);
        std::cout << extended_position_description_opt->as_epd();
        std::cout.put('\n');
    }
    std::cout << std::flush;
}
