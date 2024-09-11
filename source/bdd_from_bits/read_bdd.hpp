#ifndef EGT_COMPARISON_BDD_FROM_BITS_READ_BDD_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_READ_BDD_HPP


#include <cstdio>
#include <iostream>
#include <vector>


void skip_to_eol(std::istream & in) {
    while (true) {
        int ch{in.get()};
        if (EOF == ch) {
            return;
        }
        if ('\n' == ch) {
            return;
        }
    }
}

int next_useful(std::istream & in) {
    while (true) {
        int ch{in.get()};
        switch (ch) {
        case '.':
            skip_to_eol(in);
            [[fallthrough]];
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            continue;
        default:
            return ch;
        }
    }
}

template <typename uint_type>
bool get_query(
    std::istream & in,
    std::vector<uint_type> & vars,
    unsigned int numvars
) {
    int ch{next_useful(in)};
    if (EOF == ch) {
        return false;
    }
    vars[1] = ('0' != ch);
    for (unsigned u = 2; u <= numvars; u++) {
        vars[u] = ( in.get() != '0' );
    }
    skip_to_eol(in);
    return true;
}

bool get_query(std::istream & in, bool * vars, unsigned numvars) {
    int ch{next_useful(in)};
    if (EOF == ch) {
        return false;
    }
    vars[1] = ('0' != ch);
    for (unsigned u = 2; u <= numvars; u++) {
        vars[u] = ( in.get() != '0' );
    }
    skip_to_eol(in);
    return true;
}

template <typename uint_type>
void show_query(
    std::ostream & out,
    std::vector<uint_type> const & vars,
    unsigned numvars,
    unsigned term
) {
    for (unsigned u = 1; u <= numvars; u++) {
        out << ( vars[u] ? "1" : "0" );
    }
    out << " " << term << "\n";
}

void show_query(
    std::ostream & out,
    const bool * vars,
    unsigned numvars,
    unsigned term
) {
    for (unsigned u = 1; u <= numvars; u++) {
        out << ( vars[u] ? "1" : "0" );
    }
    out << " " << term << "\n";
}


#endif // EGT_COMPARISON_BDD_FROM_BITS_READ_BDD_HPP
