#include "bdd_from_bits/file_reader_type.hpp"


#include <cstdio>


using std::fclose;
using std::fopen;
using std::snprintf;


file_reader_type::file_reader_type(const char * inpath, char compression_style) {
    if ((compression_style != 'x') and (compression_style != 'g')
     and (compression_style != 'b')) {
        // Unrecognized compression style, so treat it as uncompressed.
        compression_style = ' ';
    }

    if (' ' == compression_style) {
        pclose_F = false;
        if (inpath) {
            F = fopen(inpath, "r");
        } else {
            F = stdin;
        }
        return;
    }

    pclose_F = true;

    const char * zcat = nullptr;
    switch (compression_style) {
    case 'x':
        zcat = "xzcat";
	break;
    case 'g':
        zcat = "gzcat";
	break;
    case 'b':
        zcat = "bzcat";
	break;
    default:
        zcat = "cat";
    }

    if (nullptr == inpath) {
        F = popen(zcat, "r");
        return;
    }

    char buffer[256];
    snprintf(buffer, 256, "%s %s", zcat, inpath);
    F = popen(buffer, "r");
}


file_reader_type::~file_reader_type() {
    if (F) {
        if (pclose_F) {
            pclose(F);
        } else {
            fclose(F);
        }
    }
}
