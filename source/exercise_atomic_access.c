#include "exercise_atomic_access.h"


#include <stdatomic.h>


// Verify that C code can invoke atomic_init() and see the atomic_bool typedef.
//
// N.B.  Third-party endgame table probing code depends upon this feature.
bool exercise_atomic_access(void) {
    _Atomic _Bool temp;
    atomic_init(&temp, false);
    atomic_bool temp2;
    atomic_init(&temp2, true);
    return atomic_load(&temp2);
}
