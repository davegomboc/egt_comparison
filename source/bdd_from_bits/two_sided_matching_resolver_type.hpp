#ifndef EGT_COMPARISON_BDD_FROM_BITS_TWO_SIDED_MATCHING_RESOLVER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_TWO_SIDED_MATCHING_RESOLVER_TYPE_HPP


#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/common_operation_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"
#include "bdd_from_bits/has_common_operation_type.hpp"
#include "bdd_from_bits/two_sided_matching_operation_type.hpp"
#include "bdd_from_bits/resolver_type.hpp"


/*
  New front end
*/
class two_sided_matching_resolver_type : public resolver_type {
    has_common_operation_type * comp;
    common_operation_type * common;
    two_sided_matching_operation_type * OP;
public:
    two_sided_matching_resolver_type(bdd_forest_type & BDD, compute_table_type & ct);
    virtual ~two_sided_matching_resolver_type() override;

    virtual unsigned int invoke_specific(unsigned int root) override;
};

resolver_type * get_two_sided_matching_multiterminal_resolver(bdd_forest_type & BDD, compute_table_type & CT);


#endif // EGT_COMPARISON_BDD_FROM_BITS_TWO_SIDED_MATCHING_RESOLVER_TYPE_HPP
