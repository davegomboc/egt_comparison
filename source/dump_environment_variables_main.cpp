#include "dump_environment_variables_main.hpp"


#include <cstdlib>
#include <iostream>

#include "environment_variable_access.hpp"


int main() {
    std::cout << "stdout\n";
    std::cerr << "stderr\n";
    std::map<std::string, std::string> env{environment_variables()};
    for (auto const & [key, value]: env) {
        std::cout << key << " => " << value << '\n';
        std::cerr << key << " => " << value << '\n';
    }
    std::cout << std::flush;
    return EXIT_SUCCESS;
}
