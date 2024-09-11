#ifndef EGT_COMPARISON_BDD_FROM_BITS_HAS_COMMON_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_HAS_COMMON_OPERATION_TYPE_HPP


#include <cassert>

#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/bdd_operation_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"


class has_common_operation_type : public bdd_operation_type {
public:
    has_common_operation_type(bdd_forest_type & bdd, compute_table_type & ct);

    bool apply(unsigned P, unsigned Q);
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_HAS_COMMON_OPERATION_TYPE_HPP
