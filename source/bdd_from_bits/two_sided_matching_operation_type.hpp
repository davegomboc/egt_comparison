#ifndef EGT_COMPARISON_BDD_FROM_BITS_TWO_SIDED_MATCHING_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_TWO_SIDED_MATCHING_OPERATION_TYPE_HPP


#include "bdd_from_bits/bdd_operation_type.hpp"
#include "bdd_from_bits/common_operation_type.hpp"
#include "bdd_from_bits/has_common_operation_type.hpp"


//
// Top down, make redundant nodes based on "compare" operation
//
class two_sided_matching_operation_type : public bdd_operation_type {
    unsigned int sequence;
    unsigned short topL;
    unsigned short botL;
    has_common_operation_type & COMP;
    common_operation_type & COMMON;
public:
    two_sided_matching_operation_type(bdd_forest_type & bdd, compute_table_type & ct, has_common_operation_type & comp, common_operation_type & common);
    virtual ~two_sided_matching_operation_type() { }

    inline void set_levels(unsigned short toplevel, unsigned short botlevel) {
        topL = toplevel;
        botL = botlevel;
        sequence = (static_cast<unsigned int>(topL) << 16) | botL;
    }

    unsigned int apply(unsigned int P);

    virtual void vtable_pinning_method() const;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_TWO_SIDED_MATCHING_OPERATION_TYPE_HPP
