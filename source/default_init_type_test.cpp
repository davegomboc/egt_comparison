#include "default_init_type_test.hpp"


#include <algorithm>
#include <vector>

#include "default_init_type.hpp"


TEST(DefaultInitType, UseWithVectorUnsignedIntAvoidsMemoryZeroing) {
    std::size_t buffer_size{1};
    bool found_nonzero_element{};
    while (not found_nonzero_element) {
        std::vector<unsigned int, default_init_type<unsigned int>>
         not_necessarily_zeroed(buffer_size);
        found_nonzero_element = (not_necessarily_zeroed.end() !=
         std::find_if(not_necessarily_zeroed.begin(),
         not_necessarily_zeroed.end(),
         [](unsigned int & value) {
            bool const result{0 != value};
            value = 0xdeadbeef;
            return result;
         }));
        buffer_size <<= 1;
    }
}
