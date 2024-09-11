#include "execution_measurement_test.hpp"


#include <cstddef>
#include <iostream>
#include <optional>


TEST(ExecutionMeasurement, AvailableBytes) {
    std::optional<std::size_t> const bytes_opt{available_bytes_opt()};
    EXPECT_NE(std::nullopt, bytes_opt);
    std::cerr << "TT: " << *bytes_opt << '\n';
}


TEST(ExecutionMeasurement, TotalBytes) {
    std::optional<std::size_t> const bytes_opt{total_bytes_opt()};
    EXPECT_NE(std::nullopt, bytes_opt);
    std::cerr << "TT: " << *bytes_opt << '\n';
}
