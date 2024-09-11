#include "query_egt.hpp"


#include <iostream>
#include <optional>

#include "prober_type.hpp"


probe_report_type query_egt_directly(tb_position_type const & tb_position) {
    std::optional<prober_type> prober_opt{prober_type::create_once_opt()};
    if (std::nullopt == prober_opt) {
        std::cerr << "EE: Unable to initialize the Syzygy probing code.\n";
        std::exit(EXIT_FAILURE);
    }
    return prober_opt->probe(tb_position);
}
