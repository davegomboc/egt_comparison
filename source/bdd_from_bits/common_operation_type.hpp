#ifndef EGT_COMPARISON_BDD_FROM_BITS_COMMON_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_COMMON_OPERATION_TYPE_HPP


#include <cassert>

#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/bdd_operation_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"


//
// Build common concretization for two nodes.
// Undefined if the comparison op returns false.
//
// Note: if we had an "illegal" value, we could use this
// in place of the comparison.
//

class common_operation_type : public bdd_operation_type {
public:
    common_operation_type(bdd_forest_type & bdd, compute_table_type & ct);

    unsigned int apply(unsigned int P, unsigned int Q);
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_COMMON_OPERATION_TYPE_HPP
