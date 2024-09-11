#include "bdd_from_bits/minterm_buffer_type.hpp"


#include <cstring>

#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/resolver_type.hpp"


minterm_buffer_type::minterm_buffer_type(
    bdd_forest_type & bdd,
    unsigned int & r,
    unsigned int bs,
    resolver_type * res
) : BDD(bdd), root(r), bufsize(bs) {
    bufptr = 0;
    minterms = 0;

    R = res;
    last_add = nullptr;

    statebuf = new char * [bufsize];
    for (unsigned u = 0; u < bufsize; u++) {
        statebuf[u] = new char[1 + BDD.get_num_vars()];
    }
}

minterm_buffer_type::~minterm_buffer_type() {
    for (unsigned u = 0; u < bufsize; u++) {
        delete[] statebuf[u];
    }
    delete[] statebuf;
}

bool minterm_buffer_type::add(char const * linebuf, char term) {
    BDD.varray2minterm(linebuf, statebuf[bufptr]);
    statebuf[bufptr][0] = term;

    if (R) {
        if (last_add) {
            unsigned short tc = top_changed(last_add, statebuf[bufptr]);
            if (tc > R->topLevel()) {

                // Flush before bufptr
                if (bufptr) {
                    root = BDD.union_minterms(root, statebuf, bufptr);
                    minterms += bufptr;
                }

                // Invoke Resolver
                unsigned short toplev = R->topLevel();
                R->set_levels(tc-1, 1);
                root = R->invoke(root);
                R->set_levels(toplev, 1);

                if (bufptr) {
                    // Save state
                    memcpy(statebuf[0], statebuf[bufptr], 1+BDD.get_num_vars());
                }
                last_add = statebuf[0];
                bufptr = 1;

                return true;
            }
        }
        last_add = statebuf[bufptr];
    }

    if (++bufptr < bufsize) return false;

    root = BDD.union_minterms(root, statebuf, bufptr);
    minterms += bufptr;
    bufptr = 0;

    return true;
}

void minterm_buffer_type::flush() {
    if (bufptr) {
        root = BDD.union_minterms(root, statebuf, bufptr);
        minterms += bufptr;
        bufptr = 0;
    }
}

unsigned short minterm_buffer_type::top_changed(char const * state1, char const * state2) const {
    for (unsigned short L = BDD.get_num_vars(); L; L--) {
        if (state1[L] != state2[L]) return L;
    }
    return 0;
}
