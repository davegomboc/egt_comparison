#ifndef EGT_COMPARISON_BDD_FROM_BITS_ONE_SIDED_MATCHING_RESOLVER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_ONE_SIDED_MATCHING_RESOLVER_TYPE_HPP


#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/compare_operation_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"
#include "bdd_from_bits/one_sided_matching_operation_type.hpp"
#include "bdd_from_bits/resolver_type.hpp"


class one_sided_matching_resolver_type : public resolver_type {
public:
    one_sided_matching_resolver_type(bdd_forest_type & BDD, compute_table_type & ct);
    virtual ~one_sided_matching_resolver_type() override;

    virtual unsigned int invoke_specific(unsigned int root) override;

private:
    compare_operation_type * comp;
    one_sided_matching_operation_type * OP;
};

resolver_type * get_one_sided_matching_multiterminal_resolver(bdd_forest_type & BDD, compute_table_type & CT);


#endif // EGT_COMPARISON_BDD_FROM_BITS_ONE_SIDED_MATCHING_RESOLVER_TYPE_HPP
