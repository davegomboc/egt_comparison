#ifndef EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_RESOLVER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_RESOLVER_TYPE_HPP


#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"
#include "bdd_from_bits/restrict_operation_type.hpp"
#include "bdd_from_bits/resolver_type.hpp"


class restrict_resolver_type : public resolver_type {
public:
    restrict_resolver_type(bdd_forest_type & BDD, compute_table_type & ct);
    virtual ~restrict_resolver_type() override;

    virtual unsigned int invoke_specific(unsigned int root) override;

private:
    restrict_operation_type * OP;
};

resolver_type * get_restrict_multiterminal_resolver(bdd_forest_type & BDD, compute_table_type & CT);


#endif // EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_RESOLVER_TYPE_HPP
