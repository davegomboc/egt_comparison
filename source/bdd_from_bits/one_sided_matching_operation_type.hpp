#ifndef EGT_COMPARISON_BDD_FROM_BITS_ONE_SIDED_MATCHING_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_ONE_SIDED_MATCHING_OPERATION_TYPE_HPP


#include "bdd_from_bits/bdd_operation_type.hpp"
#include "bdd_from_bits/compare_operation_type.hpp"


//
// Top down, make redundant nodes based on "compare" operation
//
class one_sided_matching_operation_type : public bdd_operation_type {
     unsigned int sequence;
     unsigned short topL;
     unsigned short botL;
     compare_operation_type & COMP;
public:
    one_sided_matching_operation_type(
        bdd_forest_type & bdd,
	compute_table_type & ct,
	compare_operation_type & comp);

    inline void set_levels(unsigned short toplevel, unsigned short botlevel) {
        topL = toplevel;
        botL = botlevel;
        sequence = (unsigned(topL) << 16) | botL;
    }

public:
    unsigned int apply(unsigned int P);
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_ONE_SIDED_MATCHING_OPERATION_TYPE_HPP
