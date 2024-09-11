#include "bdd_from_bits/build_bdd_main.hpp"


#include <cstdlib>
#include <fstream>
#include <sstream>

#include "bdd_from_bits/build_bdd.hpp"
#include "bdd_from_bits/command_line_argument_processing.hpp"
#include "bdd_from_bits/compute_table_type.hpp"
#include "bdd_from_bits/exception_neutrality.hpp"
#include "bdd_from_bits/parser_type.hpp"


using namespace std;


// **********************************************************************
//
// Main program
//
// **********************************************************************

int main(int argc, char ** argv) {
    //
    // Set up command line options
    //
    options_manager_type OM('h', "(options)");
    switches_type S;
    init_switches(OM, S);

    //
    // Process command line switches
    //
    int argproc = OM.parse_switches(argc, argv);
    if (argproc < argc) {
        cout << "Unknown switch: " << argv[argproc] << "\n\n";
        cout << "Run with -h to see valid command line options.\n";
        return EXIT_FAILURE;
    }

    //
    // Open input file
    //
    parser_type * P;
    if (S.input_file) {
        P = parser_type::new_parser(S.input_file);
        cerr << "Input from " << S.input_file << "\n";
    } else {
        // TBD option for this
        P = parser_type::new_parser('p', ' ');
        cerr << "Input from stdin\n";
    }
    if (nullptr == P) {
        bdd_terminate("Couldn't open input file.");
    }

    //
    // Read header portion of input
    //
    unsigned int inbits, outbits, nmt;
    P->read_header(inbits, outbits, nmt);
    if (0 == inbits) {
        bdd_terminate("Couldn't determine input bits.");
    }
    fprintf(stderr, "Header information:\n");
    report(inbits, "Input bits\n");
    report(outbits, "Output bits\n");
    report(nmt, "Expected minterms\n");

    unsigned int range = 0;
    switch (S.encoding) {
    case MTERM:
        range = 0x01 << outbits;
        fprintf(stderr, "Multi-terminal encoding; range is 0..%u\n", range - 1);
        break;

    case BTERM:
        range = 3;
        fprintf(stderr, "Bitwise encoding; range of each is 0..%u\n", range - 1);
        break;

    case DCBIN:
        range = 2;
        fprintf(stderr, "Using %u BDDs for care/don't care plus bits\n", 1 + outbits);
        break;

    default:
        bdd_terminate("Unknown encoding scheme.");
    }

    //
    // Build Variable ordering mapping
    // map[i] tells the BDD level for input variable i
    //
    unsigned int * map = nullptr;
    if (S.reorder) {
        stringstream ss(S.reorder);
        map = build_reorder(inbits, ss);
    } else if (S.reord_file) {
        ifstream F(S.reord_file);
        if (not F) {
            bdd_terminate("Couldn't open order file");
	}
        map = build_reorder(inbits, F);
    } else {
        map = new unsigned[1 + inbits];
        for (unsigned b = 0; b <= inbits; b++) map[b] = b;
    }

    //
    // Check ordering mapping
    //
    check_reorder(inbits, map);

    //
    // Build BDD forest and CT
    //
    compute_table_type CT(S.ctsize);
    bdd_forest_type BDD(static_cast<unsigned short>(inbits), map, static_cast<unsigned char>(range));
    BDD.set_CT(&CT);

    unsigned int * roots = new unsigned int[1 + outbits];
    for (unsigned int i = 0; i <= outbits; ++i) roots[i] = 0;

    S.gtimer = nmt / (S.gcount+1);
    S.gtimer += 2;
    if (MTERM == S.encoding) {
        build_multiterminal_bdd(P, S, BDD, CT, roots[0]);
    } else {
        build_bitwise_bdd(P, S, BDD, roots, 1 + outbits);
    }
    delete P;

    //
    // Concretize BDD
    //
    if (DCBIN != S.encoding) {
        concretize(S, BDD, CT, roots, 1 + outbits);
    } else {
        // nothing yet
    }

    //
    // Verify BDD (if switch is set)
    //
    if (MTERM == S.encoding) {
        if (!verify_mt(BDD, roots[0], S.verify_file)) {
            return EXIT_FAILURE;
        }
    }

    if (S.show_minterms) {
        if (MTERM == S.encoding) {
            cout << "Final minterms:\n";
            BDD.show_minterms(cout, roots[0]);
        }
    }

    if (S.write_file) {
        ofstream outf(S.write_file);
        if (!outf) {
            fprintf(stderr, "Couldn't write to file %s\n", S.write_file);
        } else {
            if (MTERM == S.encoding) {
                BDD.write_exch(outf, roots[0]);
            } else {
                BDD.write_exch(outf, roots, 1+outbits);
            }
        }
    }

    return EXIT_SUCCESS;
}
