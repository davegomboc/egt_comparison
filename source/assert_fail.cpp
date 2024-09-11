#include "assert_fail.hpp"


#include <cstdlib>
#include <cstdio>

#include "stack_trace.hpp"


extern "C" {

[[noreturn]] void __assert_fail(
    const char * assertion,
    const char * file,
    unsigned int line,
    const char * function
) {
    if (nullptr == function) {
        std::printf("%s:%u Assertion `%s' failed.\n", file, line, assertion);
    } else {
        std::printf("%s:%u: %s: Assertion `%s' failed.\n", file, line, function,
         assertion);
    }
    dump_call_stack(stderr);
    std::abort();
}

} // extern "C"
