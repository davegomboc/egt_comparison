#include "bdd_from_bits/parser_type.hpp"


#include <cstdio>
#include <cstring>

#include "bdd_from_bits/bin_parser_type.hpp"
#include "bdd_from_bits/exception_neutrality.hpp"
#include "bdd_from_bits/file_reader_type.hpp"
#include "bdd_from_bits/pla_parser_type.hpp"


//#define EGT_COMPARISON_BDD_FROM_BITS_PARSER_TYPE_DEBUG_INPUT_TYPE


using std::fflush;
using std::fprintf;
using std::printf;
using std::strncmp;


parser_type::parser_type(file_reader_type * fr) {
    FR = fr;
    if (nullptr == FR) {
        bdd_terminate("null file reader");
    }
}

parser_type::~parser_type() {
    delete FR;
}

bool matches(const char * ext, char format, char comp) {
    const char * ext1;
    const char * ext2;
    unsigned ext1len;

    switch (format) {
    case 'b':
        ext1 = ".plb";
        ext1len = 4;
        break;
    default:
        ext1 = ".pla";
        ext1len = 4;
        break;
    }

    switch (comp) {
    case 'x':
        ext2 = ".xz";
        break;
    case 'g':
        ext2 = ".gz";
        break;
    case 'b':
        ext2 = ".bz2";
        break;
    default:
        ext2 = "";
        break;
    }

    if (strncmp(ext1, ext, ext1len)) {
        return false;
    }
    return 0 == strcmp(ext2, ext+ext1len);
}

bool file_type(const char * pathname, char & format, char & comp) {
    if (nullptr == pathname) return false;
    const char * formats = "pb";
    const char * comps   = " xgb";

    for (unsigned int i{}; pathname[i]; ++i) {
        if ('.' != pathname[i]) {
            continue;
        }
        for (unsigned int f{}; formats[f]; f++) {
            format = formats[f];
            for (unsigned int c{}; comps[c]; c++) {
                comp = comps[c];
                if (matches(pathname+i, format, comp)) {
                    return true;
                }
            }
        }
    }
    return false;
}

parser_type * parser_type::new_parser(
    const char * pathname,
    char format,
    char compression
) {
#ifdef EGT_COMPARISON_BDD_FROM_BITS_PARSER_TYPE_DEBUG_INPUT_TYPE
    fprintf(stderr, "Building parser %s '%c' '%c'\n", pathname, format,
     compression);
#endif
    file_reader_type * fr = new file_reader_type(pathname, compression);
    if (!fr->fok()) {
        delete fr;
        return nullptr;
    }

    switch (format) {
    case 'p':
    case 'P':
        return new pla_parser_type(fr);
    case 'b':
    case 'B':
        return new bin_parser_type(fr);
    default:
        fprintf(stderr, "No parser for format %c\n", format);
        return nullptr;
    }
}

parser_type * parser_type::new_parser(char format, char compression) {
    return new_parser(nullptr, format, compression);
}

parser_type * parser_type::new_parser(const char * pathname) {
    char fmt;
    char comp;
    if (not file_type(pathname, fmt, comp)) {
        fprintf(stderr, "Couldn't determine file type for input pathname %s\n",
         pathname);
        return nullptr;
    }
    return new_parser(pathname, fmt, comp);
}

void parser_type::debug(bool show_header, bool show_minterms, bool show_summary) {
    unsigned int ib, ob, nmt;
    read_header(ib, ob, nmt, show_header ? stdout : nullptr);

    if (show_summary) {
        printf("input  bits: %u\n", ib);
        printf("output bits: %u\n", ob);
        printf("expected #minterms: %u\n", nmt);
        fflush(stdout);
    }

    char * minterm = new char[ib + 2];
    char term;
    unsigned int i;
    for (i = 0; read_minterm(minterm, term); ++i) {
        int traw = term - '0';
        if (show_minterms) {
            printf("Minterm: %s => %c%c%c (terminal %c)\n",
             minterm,
             (traw & 4) ? '1' : '0',
             (traw & 2) ? '1' : '0',
             (traw & 1) ? '1' : '0',
             term);
        }
    }
    if (show_summary) {
        printf("actual   #minterms: %u\n", i);
    }
    delete[] minterm;
}
