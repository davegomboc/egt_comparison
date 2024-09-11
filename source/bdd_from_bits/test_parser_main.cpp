#include <cstdlib>
#include <iostream>

#include "exception_neutrality.hpp"
#include "parser_type.hpp"


using std::cerr;


int usage(const char * exe) {
    cerr << "Usage: " << exe << " (options) (infile)\n";
    cerr << "\nOptions:\n";
    cerr << "\t -h: don't show header\n";
    cerr << "\t -H: show header\n";
    cerr << "\t -m: don't show minterms\n";
    cerr << "\t -M: show minterms\n";
    cerr << "\t -s: don't show summary\n";
    cerr << "\t -S: show summary\n";
    cerr << "\n";
    return EXIT_FAILURE;
}

int main(int argc, char ** argv) {
    //
    // Process command line arguments
    //
    bool show_header = true;
    bool show_minterms = true;
    bool show_summary = true;
    const char * infile = nullptr;

    for (int i = 1; i < argc; ++i) {
        if ('-' != argv[i][0]) {
            if (infile) return usage(argv[0]);
            infile = argv[i];
            continue;
        }
        if (0 == argv[i][1]) return usage(argv[0]);
        if (0 != argv[i][2]) return usage(argv[0]);
        switch (argv[i][1]) {
        case 'h':
            show_header = false;
	    continue;
        case 'H':
            show_header = true;
	   continue;
        case 'm':
            show_minterms = false;
            continue;
        case 'M':
            show_minterms = true;
            continue;
        case 's':
            show_summary = false;
            continue;
        case 'S':
            show_summary = true;
            continue;
        default:
            return usage(argv[0]);
        }
    }

    //
    // Open input file
    //
    parser_type * P;
    if (infile) {
        P = parser_type::new_parser(infile);
        cerr << "Input from " << infile << "\n";
    } else {
        // TBD option for this
        P = parser_type::new_parser('p', ' ');
        cerr << "Input from stdin\n";
    }
    if (nullptr == P) {
        bdd_terminate("Couldn't open input file.");
    }
    P->debug(show_header, show_minterms, show_summary);
    delete P;

    return EXIT_SUCCESS;
}
