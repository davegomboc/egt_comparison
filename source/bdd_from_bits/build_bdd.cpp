#include "bdd_from_bits/build_bdd.hpp"


#include <cstdio>
#include <istream>

#define EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_SHOW_TIMING

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_SHOW_TIMING
#include <sys/time.h>
#include <ctime>
#endif // EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_SHOW_TIMING


#include "bdd_from_bits/exception_neutrality.hpp"
#include "bdd_from_bits/minterm_buffer_type.hpp"
#include "bdd_from_bits/one_sided_matching_resolver_type.hpp"
#include "bdd_from_bits/restrict_resolver_type.hpp"
#include "bdd_from_bits/two_sided_matching_resolver_type.hpp"


using namespace std;


static enum_argument_type::value_type ALGOS[NUMALGS];
static enum_argument_type::value_type ENCODINGS[NUMENCS];


resolver_type * new_resolver(unsigned int type, bdd_forest_type & bdd, compute_table_type & CT) {
    switch (type) {
    case RESTR:
        return get_restrict_multiterminal_resolver(bdd, CT);

    case OSM:
        return get_one_sided_matching_multiterminal_resolver(bdd, CT);

    case TSM:
        return get_two_sided_matching_multiterminal_resolver(bdd, CT);

    default:
        // none
        return nullptr;
    }
}

// **********************************************************************
//
// Fancy output
//
// **********************************************************************

void report(unsigned long value, char const * item) {
    bool printed = false;
    fprintf(stderr, "  ");
    unsigned long expo = 1000;
    expo *= 1000;
    expo *= 1000;
    expo *= 1000;
    while (expo>1) {
        unsigned long digits = value / expo;
        if (printed) {
            fprintf(stderr, ",%03lu", digits);
            value %= expo;
        } else {
            if (digits) {
                fprintf(stderr, " %3lu", digits);
                value %= expo;
                printed = true;
            } else {
                fprintf(stderr, "    ");
            }
        }
        expo /= 1000;
    }
    if (printed) {
        fprintf(stderr, ",%03lu", value);
    } else {
        fprintf(stderr, " %3lu", value);
    }
    if (item) {
        fprintf(stderr, "  %s", item);
    }
}

// **********************************************************************
//
// Helpers for various switches
//
// **********************************************************************

unsigned int * build_reorder(unsigned int nbits, istream & in) {
    unsigned int * map = new unsigned int [nbits+1];
    // always!
    map[0] = 0;
    unsigned level;
    in >> level;
    map[1] = level;
    for (unsigned b = 2; b <= nbits; b++) {
        char comma;
        in >> comma;
        if (comma != ',') {
            bdd_terminate("Expected , in reorder list");
        }
        in >> level;
        map[b] = level;
    }

    return map;
}

void check_reorder(unsigned int nbits, unsigned int const * map) {
    bool * saw_value = new bool[nbits+1];
    for (unsigned i = 0; i <= nbits; i++) saw_value[i] = false;

    if (map[0] != 0) {
        bdd_terminate("Invalid map[0], how?");
    }

    for (unsigned int i = 1; i <= nbits; i++) {
        if ((map[i] < 1) || (map[i] > nbits)) {
            bdd_terminate("Invalid level in reorder");
        }
        if (saw_value[map[i]]) {
            bdd_terminate("Duplicate level in reorder");
        }
        saw_value[map[i]] = true;
    }

    delete[] saw_value;
}

// **********************************************************************
//
// Timing stuff
//
// **********************************************************************

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_SHOW_TIMING

inline long get_time_in_us() {
    timeval the_time;
    gettimeofday(&the_time, nullptr);
    return the_time.tv_sec * 1000000 + the_time.tv_usec;
}

inline void show_elapsed(char const * what, long time) {
    long const elapsed_sec = time / 1000000;
    long const elapsed_usec = time % 1000000;
    fprintf(stderr, "%s took %ld.%06ld seconds\n", what, elapsed_sec, elapsed_usec);
}

#else

inline long get_time_in_us() {
    return 0;
}

inline void show_elapsed(const char*, long) {}

#endif

// **********************************************************************
//
// Build MT BDD.
//
// **********************************************************************
void build_multiterminal_bdd(
    parser_type * P,
    switches_type const & S,
    bdd_forest_type & bdd,
    compute_table_type & CT,
    unsigned int & root
) {
    char * linebuf = new char[5 + bdd.get_num_vars()];

    resolver_type * R = new_resolver(S.DALG, bdd, CT);
    if (R) R->set_levels(static_cast<unsigned short>(S.Dlevel), 1);
    if (S.DALG) fprintf(stderr, "Concretizing using '%s' above level %u\n", ALGOS[S.DALG].get_str(), S.Dlevel);

    //
    // Build buffer (TBD: for -r, build a buffer for each range value)
    //
    fprintf(stderr, "Using buffer of size %u\n", S.bufsize);
    minterm_buffer_type B(bdd, root, S.bufsize, R);

    //
    // Build BDD from minterms
    //
    fprintf(stderr, "Building BDD...\n");
    fflush(stderr);

    long start_time = get_time_in_us();

    unsigned int num_gcs = 0;
    unsigned int unions = 0;
    unsigned int gtimer = S.gtimer;
    for (;;) {
        char term;
        if (! P->read_minterm(linebuf, term)) break;
        if ('0' == term) continue;

        unions += B.add(linebuf, term);

        if (--gtimer) continue;
        fprintf(stderr, "  gc at %lu minterms\n", B.get_num_minterms());
        gtimer = S.gtimer;
        ++num_gcs;
        bdd.mark_and_sweep(root);
    } // for loop

    //
    // Add any remaining buffer
    //
    B.flush();
    ++unions;
    ++num_gcs;
    unsigned int count = bdd.mark_and_sweep(root);

    show_elapsed("BDD construction", get_time_in_us() - start_time);

    report(B.get_num_minterms(), "minterms\n");
    report(bdd.peak_nodes(), "peak BDD nodes\n");
    report(bdd.peak_nodes() * sizeof(bdd_node_type), "peak BDD bytes\n");
    report(count, "final BDD nodes\n");
    report(count * sizeof(bdd_node_type), "final BDD bytes\n");
    report(unions, "unions\n");
    report(num_gcs, "garbage collections\n");
    if (R) report(R->num_calls(), "resolutions\n");
    fflush(stderr);

/*
    unsigned low[16], high[16];
    count = bdd.count_pattern(low, high);

    for (unsigned t = 0; t < bdd.get_num_terminals(); t++) {
        if (low[t]) {
            report(low[t], " low ");
            fprintf(stderr, "%u nodes\n", t);
        }
    }
    for (unsigned t = 0; t < bdd.get_num_terminals(); t++) {
        if (high[t]) {
            report(high[t], " high ");
            fprintf(stderr, "%u nodes\n", t);
        }
    }
    report(count, "  nodes\n");
*/

    delete[] linebuf;
    delete R;
}

// **********************************************************************
//
// Concretize BDD(s) with "don't care" as value 0
//
// **********************************************************************

void concretize(
    switches_type const & S,
    bdd_forest_type & bdd,
    compute_table_type & CT,
    unsigned int * roots,
    unsigned int nroots
) {
    long start_time = get_time_in_us();

    resolver_type * R = new_resolver(S.UALG, bdd, CT);
    if (nullptr == R) {
        // Nothing to do.
        return;
    }

    unsigned int count = 0;
    unsigned short i;
    unsigned short topL = bdd.get_num_vars();
    unsigned short botL = 1;

    fprintf(stderr, "Concretizing using '%s'", ALGOS[S.UALG].get_str());
    if (S.Ubatch > 0) {
        fprintf(stderr, ", bottom up, batch size %d", S.Ubatch);
        topL = static_cast<unsigned short>(S.Ubatch);
    }
    if (S.Ubatch < 0) {
        fprintf(stderr, ", top down, batch size %d", -S.Ubatch);
        botL = static_cast<unsigned short>(topL + S.Ubatch + 1);
    }
    fprintf(stderr, "...\n");
    fflush(stderr);
    for (unsigned ri = 0; ri < nroots; ++ri) {
        if (0 == roots[ri]) continue;
        fprintf(stderr, "    BDD #%u\n", ri);
        for (i = 1; ; ++i) {
            if (R) {
                R->set_levels(topL, botL);
                roots[ri] = R->invoke(roots[ri]);
            }
            count = bdd.mark_and_sweep(roots, nroots);
            fprintf(stderr, "\tIter %3u, levels %3d to %3d.  ", i, topL, botL);
            report(count, " nodes\n");
            if (0 == S.Ubatch) break;
            topL += static_cast<unsigned short>(S.Ubatch);
            botL += static_cast<unsigned short>(S.Ubatch);
            if (botL > bdd.get_num_vars()) break;
            if (topL > bdd.get_num_vars()) {
                topL = bdd.get_num_vars();
            }
            if (topL < 1) break;
            if (botL < 1) botL = 1;
        }
    }

    show_elapsed("BDD concretization", get_time_in_us() - start_time);

    report(count, "nodes in concretized BDD\n");
    report(count * sizeof(bdd_node_type), "bytes for concretized BDD\n");
    fflush(stderr);

    delete R;
}

// **********************************************************************
//
// Verify MT BDD.  Checks BDD against minterms.
//
// **********************************************************************

bool verify_mt(bdd_forest_type const & bdd, unsigned int root, char const * vfile) {
    if (nullptr == vfile) return true;

    parser_type * P = parser_type::new_parser(vfile);
    if (nullptr == P) {
        bdd_terminate("Couldn't open input file.");
    }

    fprintf(stderr, "Verifying against %s...\n", vfile);
    fflush(stderr);
    long start_time = get_time_in_us();

    unsigned int inbits, outbits, nmt;
    P->read_header(inbits, outbits, nmt);

    if (inbits != bdd.get_num_vars()) {
        bdd_terminate("verify fail: different number of variables");
    }
    char * minterm = new char[2 + inbits];
    minterm[inbits+1] = 0;

    unsigned long checked = 0;
    unsigned long zeroed = 0;
    char * linebuf = new char[5 + bdd.get_num_vars()];

    for (;;) {
        char value;
        if (not P->read_minterm(linebuf, value)) break;
        if ('0' == value) {
            zeroed++;
            continue;
        }

        bdd.varray2minterm(linebuf, minterm);
        char term = bdd.evaluate(root, minterm);
        if (value != term) {
            cout << "Verification failure on minterm #" << checked << "\n";
            cout << "\tInput line: " << linebuf << " (value " << value << ")\n";
            cout << "\tMinterm   : " << minterm+1 << "\n";
            cout << "\tTerminal  : " << term << "\n";
            delete[] minterm;
            delete[] linebuf;
            delete P;
            return false;
        }

        checked++;
    } // for loop
    show_elapsed("Verification", get_time_in_us() - start_time);
    report(checked, "minterms checked\n");
    report(zeroed , "minterms skipped (zero terminal)\n");
    report(nmt    , "minterms expected\n");
    delete[] minterm;
    delete[] linebuf;
    delete P;
    return true;
}

// **********************************************************************
//
// Build bitwise BDD.
//
// **********************************************************************

void build_bitwise_bdd(
    parser_type * P,
    switches_type const & S,
    bdd_forest_type & bdd,
    unsigned int roots[],
    unsigned int nroots
) {
    char * linebuf = new char[5 + bdd.get_num_vars()];

    bool dontcare_map = (DCBIN == S.encoding);
    // No resolution during construction.

    //
    // Build buffers
    //
    fprintf(stderr, "Using buffers of size %u\n", S.bufsize);
    minterm_buffer_type ** B;
    B = new minterm_buffer_type * [nroots];
    for (unsigned int i = 0; i < nroots; ++i) {
        B[i] = new minterm_buffer_type(bdd, roots[i], S.bufsize, nullptr);
    }

    //
    // Build BDDs from minterms
    //
    fprintf(stderr, "Building BDDs...\n");
    fflush(stderr);

    long start_time = get_time_in_us();

    unsigned int num_gcs = 0;
    unsigned int unions = 0;
    unsigned int gtimer = S.gtimer;
    for (;;) {
        char term;
        if (not P->read_minterm(linebuf, term)) break;
        if ('0' == term) continue;
        unsigned int const traw = static_cast<unsigned int>(term - '0');

        if (dontcare_map) {
            unions += B[0]->add(linebuf, '1');
        }
        unsigned int mask = 0x01;
        for (unsigned i = 1; i < nroots; ++i) {
            if (mask & traw) {
                unions += B[i]->add(linebuf, '1');
            } else {
                if (not dontcare_map) {
                    unions += B[i]->add(linebuf, '2');
                }
            }
            mask *= 2;
        }

        if (--gtimer) continue;
        fprintf(stderr, "  gc at %lu minterms\n", B[1-dontcare_map]->get_num_minterms());
        gtimer = S.gtimer;
        ++num_gcs;
        bdd.mark_and_sweep(roots, nroots);
    } // for loop

    //
    // Add any remaining buffers
    //
    for (unsigned int i = 0; i < nroots; ++i) {
        B[i]->flush();
    }
    ++unions;
    ++num_gcs;
    unsigned int count = bdd.mark_and_sweep(roots, nroots);

    show_elapsed("BDD construction", get_time_in_us() - start_time);

    report(B[0]->get_num_minterms(), "care/don't care minterms\n");
    for (unsigned i = 1; i < nroots; ++i) {
        report(B[i]->get_num_minterms(), "bit ");
        fprintf(stderr, "%u minterms\n", i);
    }
    report(bdd.peak_nodes(), "peak BDD nodes (forest)\n");
    report(bdd.peak_nodes() * sizeof(bdd_node_type), "peak BDD bytes (forest)\n");
    report(count, "final BDD nodes (forest)\n");
    report(count * sizeof(bdd_node_type), "final BDD bytes (forest)\n");
    report(unions, "unions\n");
    report(num_gcs, "garbage collections\n");
    fflush(stderr);

    delete[] linebuf;
    for (unsigned i = 0; i < nroots; ++i) delete B[i];
    delete[] B;
}


// **********************************************************************
//
// Set up options
//
// **********************************************************************


void init_switches(options_manager_type & OM, switches_type & S) {
    //
    // Set up algorithm names and documentation
    ALGOS[NONE].set("none", "Leave any undefined values.");
    ALGOS[RESTR].set("restr", "'restrict'.  Nodes with an undefined\nchild become redundant.");
    ALGOS[OSM].set("osm", "Top-down, check for one-sided matches.");
    ALGOS[TSM].set("tsm", "Top-down, check for two-sided matches.");

    //
    // Set up encodings
    ENCODINGS[MTERM].set("mt", "Multi-terminal BDD (function range plus don't care).");
    ENCODINGS[BTERM].set("bt", "One BDD for each bit, outputs are 0 for don't care,\n1 for bit set, 2 for bit not set.");
    ENCODINGS[DCBIN].set("dcb", "One BDD for care/don't care, one BDD per output bit;\noutputs purely binary.");


    // -m
    OM.add_option('m', new letter_option_type(
      S.show_minterms, "Display minterms when done."
    ));
    S.show_minterms = 0;

    // -p
    OM.add_option('p', new letter_option_type(
      S.show_progress, "Show progress."
    ));
    S.show_progress = 0;

    // -i
    OM.add_option('i', new bdd_option_type(
      "Read input from file.  If missing, read from\n"
      "standard input.  If extension .xz is present,\n"
      "input file is piped through xzcat.",
      1,
      new string_argument_type(" file", S.input_file, nullptr)
    ));
    S.input_file = nullptr;

    // -v
    OM.add_option('v', new bdd_option_type(
      "Verify resulting BDD(s) against file, which\n"
      "has exactly the same format as the input.",
      1,
      new string_argument_type(" file", S.verify_file, nullptr)
    ));
    S.verify_file = nullptr;

    // -o
    OM.add_option('o', new bdd_option_type(
      "Specifiy variable order, as a comma-separated list of\n"
      "levels.  If omitted, defaults to 1,2,3,...,L\n"
      "Note that level 1 is the bottom, level L is the top.",
      1,
      new string_argument_type(" order", S.reorder, nullptr)
    ));
    S.reorder = nullptr;

    // -O
    OM.add_option('O', new bdd_option_type(
      "Read the variable order from a file.  The file format\n"
      "is similar to -o except whitespace is allowed.",
      1,
      new string_argument_type(" file", S.reord_file, nullptr)
    ));
    S.reord_file = nullptr;


    // -b
    OM.add_option('b', new bdd_option_type(
      "Number of cubes to buffer, before adding to BDD.",
      1,
      new unsigned_argument_type(" bufsize", S.bufsize, nullptr)
    ));
    S.bufsize = 1024;

    // -c
    OM.add_option('c', new bdd_option_type(
      "Size of compute table.",
      1,
      new unsigned_argument_type(" ctsize", S.ctsize, nullptr)
    ));
    S.ctsize = 1024*1024;

    // -g
    OM.add_option('g', new bdd_option_type(
      "Evenly spread out this many GC runs during BDD construction.",
      1,
      new unsigned_argument_type(" gcount", S.gcount, nullptr)
    ));
    S.gcount = 5;

    // -d
    OM.add_option('d', new bdd_option_type(
      "Attempt to resolve undefined values, during construction.",
      2,
      new enum_argument_type("alg", S.DALG, "specifies the algorithm to use, run in batches.", NUMALGS,
        ALGOS[0].link(), ALGOS[1].link(), ALGOS[2].link(), ALGOS[3].link()
      ),
      new unsigned_argument_type("k", S.Dlevel,
        "specifies the level that triggers the resolver:\n"
        "when the minterm changes above level k, we resolve\n"
        "undefined values from level k downward."
      )
    ));
    S.DALG = NONE;
    S.Dlevel = 16;

    // -e
    OM.add_option('e', new bdd_option_type(
      "How to encode the output.",
      1,
      new enum_argument_type("enc", S.encoding, "specifies the encoder to use.", NUMENCS,
        ENCODINGS[0].link(), ENCODINGS[1].link(), ENCODINGS[2].link()
      )
    ));
    S.encoding = MTERM;

    // -u
    OM.add_option('u', new bdd_option_type(
      "How to resolve undefined values, after construction.", 2,
      new enum_argument_type("alg", S.UALG, "specifies the algorithm to use, run in batches.", NUMALGS,
        ALGOS[0].link(), ALGOS[1].link(), ALGOS[2].link(), ALGOS[3].link()
      ),
      new signed_argument_type("B", S.Ubatch,
        "specifies the batch size.\n"
        "Positive B: run bottom up, B levels at a time\n"
        "Negative B: run top down, -B levels at a time\n"
        "Zero B: run for all levels at once"
      )
    ));
    S.UALG = NONE;
    S.Ubatch = 0;

    // -w
    OM.add_option('w', new bdd_option_type(
      "Write the final BDD to the specified file,\n"
      "in an exchange format.",
      1,
      new string_argument_type(" file", S.write_file, nullptr)
    ));
    S.write_file = nullptr;
}
