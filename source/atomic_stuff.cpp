#include "atomic_stuff.hpp"


#include <cstddef>


template void initialize_if_necessary<std::size_t>(
    std::vector<std::atomic<std::size_t>> &,
    std::size_t);
