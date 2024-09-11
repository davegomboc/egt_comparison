#ifndef EGT_COMPARISON_BDD_FROM_BITS_RESOLVER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_RESOLVER_TYPE_HPP


#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"


class resolver_type {
public:
    resolver_type(bdd_forest_type & BDD, compute_table_type & ct);
    virtual ~resolver_type();

    inline void set_levels(unsigned short top, unsigned short bot) {
        toplevel = top;
        botlevel = bot;
    }

    inline unsigned short topLevel() const { return toplevel; }
    inline unsigned short botLevel() const { return botlevel; }

    inline unsigned int invoke(unsigned int root) {
        ++invoke_count;
        return invoke_specific(root);
    }

    inline unsigned int num_calls() const {
        return invoke_count;
    }

protected:
    virtual unsigned int invoke_specific(unsigned int root) = 0;

    bdd_forest_type & BDD;
    compute_table_type & CT;

private:
    unsigned short toplevel;
    unsigned short botlevel;
    unsigned int invoke_count;
};


// Implemented in alg_restr.cc
resolver_type * restrict_mt(bdd_forest_type & BDD, compute_table_type & CT);


// Implemented in alg_osm.cc
resolver_type * one_side_match_mt(bdd_forest_type & BDD, compute_table_type & CT);


// Implemeted in alg_tsm.cc
resolver_type * two_side_match_mt(bdd_forest_type & BDD, compute_table_type & CT);


#endif // EGT_COMPARISON_BDD_FROM_BITS_RESOLVER_TYPE_HPP
