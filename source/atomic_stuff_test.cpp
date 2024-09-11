#include "atomic_stuff_test.hpp"


#include <cstdint>


TEST(AtomicStuff, InitializeAtomicVectorWithZero) {
    std::vector<std::atomic<std::size_t>> atomic_vector(1000);
    initialize_if_necessary<std::size_t>(atomic_vector, 0);
    for (auto const & element: atomic_vector) {
        EXPECT_EQ(0, element);
    }
}
