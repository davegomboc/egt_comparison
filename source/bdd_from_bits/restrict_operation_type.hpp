#ifndef EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_HPP


#include "bdd_from_bits/bdd_operation_type.hpp"
#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"


class restrict_operation_type : public bdd_operation_type {
public:
    restrict_operation_type(bdd_forest_type & bdd, compute_table_type & ct, unsigned int UNK);

    inline void set_levels(unsigned short toplevel, unsigned short botlevel) {
        topL = toplevel;
        botL = botlevel;
        sequence = (static_cast<unsigned int>(topL) << 16) | botL;
    }

    unsigned int apply(unsigned int P);

    unsigned int const UNKNOWN;

private:
    unsigned int sequence;
    unsigned short topL;
    unsigned short botL;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_HPP
