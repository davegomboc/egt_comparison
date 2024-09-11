#include "bdd_from_bits/pla_parser_type.hpp"


#include "bdd_from_bits/exception_neutrality.hpp"


//#define EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_DEBUG_MINTERM


using std::fprintf;


pla_parser_type::pla_parser_type(file_reader_type * fr)
  : parser_type(fr), inbits(0), outbits(0) {}

pla_parser_type::~pla_parser_type() {}

void pla_parser_type::read_header(
    unsigned int & ib,
    unsigned int & ob,
    unsigned int & nmt,
    FILE * debug
) {
    ib = 0;
    ob = 0;
    nmt = 0;

    if (debug) {
        fprintf(debug, "Header information: \n");
    }

    // Read lines while first character is .
    //
    for (;;) {
        int next = get();
        if (EOF == next) {
            bdd_terminate("Unexpected EOF.");
        }
        if ('#' == next) {
            skip_until('\n');
            continue;
        }
        if ('.' != next) {
            unget(next);
            break;
        }

        // we got .

        next = get();
        if ('i' == next) {
            ib = read_unsigned();
            if (debug) {
                fprintf(debug, "    .i %u\n", ib);
            }
        }
        if ('o' == next) {
            ob = read_unsigned();
            if (debug) {
                fprintf(debug, "    .o %u\n", ob);
            }
        }
        if ('p' == next) {
            nmt = read_unsigned();
            if (debug) {
                fprintf(debug, "    .p %u\n", nmt);
            }
        }
        skip_until('\n');
    }

    inbits = ib;
    outbits = ob;
}

inline bool is_ws(int x) {
    return (' ' == x) || ('\t' == x) || ('\r' == x);
}

inline bool is_dash(int x) {
    return ('-' == x) || ('2' == x);
}

inline unsigned outbit2value(int x) {
    return ('1' == x) ? 1 : 0;
}

bool pla_parser_type::read_minterm(char* input_bits, char& out_terminal) {
    int c;
    for (;;) {
        c = get();
        if (EOF == c) return false;
        if ('.' == c) {
            skip_until('\n');
#ifdef EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_DEBUG_MINTERM
            fprintf(stderr, "skipped . line\n");
#endif
        }
        break;
    }
    // non . line.

    //
    // Read input bits.  It is assumed that only minterms will be present.
    //
    input_bits[0] = static_cast<char>(c);
    unsigned u;
    for (u = 1; u < inbits; ++u) {
        c = get();
        if (EOF == c) return false;
        input_bits[u] = static_cast<char>(c);
    }
    input_bits[u] = 0;

    //
    // Read output bits and build terminal value.
    //
    bool all_dash = true;
    unsigned t = 0;
#ifdef EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_DEBUG_MINTERM
        char out[1 + outbits];
        u = 0;
#endif
    for (;;) {
        c = get();
        if (EOF == c) return false;
        if ('\n' == c) break;
        if (is_ws(c)) continue;
#ifdef EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_DEBUG_MINTERM
        out[u++] = c;
#endif
        t *= 2;
        t += outbit2value(c);
        if (!is_dash(c)) all_dash = false;
    }
    if (all_dash) {
        out_terminal = '0';
    } else {
        out_terminal = static_cast<char>('0' + t);
    }

#ifdef EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_DEBUG_MINTERM
    out[u] = 0;
    fprintf(stderr, "Got line: %s => %s\n", input_bits, out);
#endif
    return true;
}
