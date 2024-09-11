#include "bdd_from_bits/bin_parser_type.hpp"


#include <cstdio>

#include "bdd_from_bits/exception_neutrality.hpp"


//#define EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_DEBUG_MINTERM
//#define EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_NO_CURSED


using std::fprintf;


bin_parser_type::bin_parser_type(file_reader_type * fr) : parser_type(fr) {
    inbits = 0;
    numf = 0;
}

bin_parser_type::~bin_parser_type() {}

void bin_parser_type::check_bitstream() {
    if (0 == bits_avail) {
        if (not read_le(bits)) {
            bdd_terminate("unexpected eof");
        }
        bits_avail = sizeof(unsigned long) * 8;
    }
}

void bin_parser_type::clear_bitstream() {
    bits_avail = 0;
}

bool bin_parser_type::read_bit1() {
    check_bitstream();
    unsigned long bp = bits & 0x01;
    bits >>= 1;
    bits_avail -= 1;
    return bp;
}

bool bin_parser_type::read_bit2() {
    check_bitstream();
    unsigned long bp = bits & 0x03;
    bits >>= 2;
    bits_avail -= 2;
    switch (bp) {
    case 1:
        return false;
    case 2:
        return true;
    default:
        bdd_terminate("unexpected bit value");
    }
    __builtin_unreachable();
}

void bin_parser_type::read_header(
    unsigned int & ib,
    unsigned int & ob,
    unsigned & nmt,
    FILE * debug
) {
    // Hard coded
    ob = 3;

    unsigned long numbits, numr, numdc;
    read_le(numbits);
    read_le(numf);  // member
    read_le(numr);
    read_le(numdc);
    nmt = static_cast<unsigned int>(numf);

    inbits = ib = static_cast<unsigned int>((numbits-ob) / 2);

    if (debug) {
        fprintf(debug, "numbits: %lu\n", numbits);
        fprintf(debug, "numf   : %lu\n", numf);
        fprintf(debug, "numr   : %lu\n", numr);
        fprintf(debug, "numdc  : %lu\n", numdc);
    }
}

inline bool bin_parser_type::read_le(unsigned long & u) {
/*
    static unsigned char raw[8];
    unsigned actual = read(raw, 8);
    if (actual < 8) return false;
    unsigned long const u0 = raw[0];
    unsigned long const u1 = raw[1];
    unsigned long const u2 = raw[2];
    unsigned long const u3 = raw[3];
    unsigned long const u4 = raw[4];
    unsigned long const u5 = raw[5];
    unsigned long const u6 = raw[6];
    unsigned long const u7 = raw[7];

    u = (u0      ) | (u1 <<  8) | (u2 << 16) | (u3 << 24) |
        (u4 << 32) | (u5 << 40) | (u6 << 48) | (u7 << 56);

    return true;
*/
    return (8 == read(&u, 8));
}

bool bin_parser_type::read_minterm(char * input_bits, char & out_terminal) {
#ifndef EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_DEBUG_MINTERM
    if (0==numf) return false;
#endif

    clear_bitstream();

    for (unsigned int i = 0; i < inbits; ++i) {
        input_bits[i] = '0' + ( read_bit2() ? 1 : 0 );
    }
    input_bits[inbits] = 0;

    unsigned char const w1 = read_bit1() ? 1 : 0;
    unsigned char w2 = read_bit1() ? 1 : 0;
    unsigned char const w3 = read_bit1() ? 1 : 0;
    if (w1 && w3) {
        out_terminal = '0';
    } else {
#ifdef EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_NO_CURSED
        if ( w2 && (w1 || w3) ) {
            w2 = 0;
        }
#endif
        out_terminal = static_cast<char>('0' + w1 * 4 + w2 * 2 + w3);
    }

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_DEBUG_MINTERM
    if (0==numf) {
        fprintf(stderr, "Ignored line: %s => %c%c%c (terminal %c)\n", input_bits,
         w3 ? '1' : '0',
         w2 ? '1' : '0',
         w1 ? '1' : '0',
         out_terminal);
        return false;
    }
    fprintf(stderr, "Got line: %s => %c%c%c (terminal %c)\n", input_bits,
     w3 ? '1' : '0',
     w2 ? '1' : '0',
     w1 ? '1' : '0',
     out_terminal);
#endif
    numf--;
    return true;
}
