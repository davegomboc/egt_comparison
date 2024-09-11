#ifndef EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_HPP


#include <istream>

#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/command_line_argument_processing.hpp"
#include "bdd_from_bits/parser_type.hpp"
#include "bdd_from_bits/resolver_type.hpp"


// concretization algorithms
unsigned int const NONE    = 0;
unsigned int const RESTR   = 1;
unsigned int const OSM     = 2;
unsigned int const TSM     = 3;
unsigned int const NUMALGS = 4;

// encodings
unsigned int const MTERM   = 0;
unsigned int const BTERM   = 1;
unsigned int const DCBIN   = 2;
unsigned int const NUMENCS = 3;

struct switches_type {
    char const * reorder;     // reorder string
    char const * reord_file;  // reorder filename
    char const * input_file;  // input filename
    char const * verify_file; // verify filename
    char const * write_file;  // write filename
    unsigned int bufsize;     // size of buffer when building from minterms
    unsigned int gcount;      // number of times to run GC while building
    unsigned int gtimer;
    unsigned int ctsize;      // compute table size
    char show_progress;       // show progress while reading input?
    char show_minterms;       // display minterms when done?
    unsigned int encoding;    // encoding scheme to use
    unsigned int DALG;        // algorithm for undefined, during
    unsigned int Dlevel;      // level number for triggering resolver, during
    unsigned int UALG;        // algorithm for resolving undefined, after
    int Ubatch;               // batch size for UALG; 0 means do all levels.
};

void build_bitwise_bdd(
    parser_type * P,
    switches_type const & S,
    bdd_forest_type & bdd,
    unsigned int roots[],
    unsigned int nroots);

void build_multiterminal_bdd(
    parser_type * P,
    switches_type const & S,
    bdd_forest_type & bdd,
    compute_table_type & CT,
    unsigned int & root);

unsigned int * build_reorder(unsigned int nbits, std::istream & in);

void check_reorder(unsigned int nbits, unsigned int const * map);

void concretize(
    switches_type const & S,
    bdd_forest_type & bdd,
    compute_table_type & CT,
    unsigned int * roots,
    unsigned int nroots);

void init_switches(options_manager_type & OM, switches_type & S);

resolver_type * new_resolver(unsigned int type, bdd_forest_type & bdd, compute_table_type & CT);

void report(unsigned long value, char const * item);

bool verify_mt(bdd_forest_type const & bdd, unsigned int root, char const * vfile);


#endif // EGT_COMPARISON_BDD_FROM_BITS_BUILD_BDD_HPP
