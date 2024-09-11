#include "bdd_from_bits/exception_neutrality.hpp"


#include <exception>
#include <iostream>


void bdd_terminate(std::string const & message) {
#if defined(__cpp_exceptions)
    throw std::runtime_error(message);
#else
    std::cerr << "EE: " << message << '\n';
    std::terminate();
#endif
}
