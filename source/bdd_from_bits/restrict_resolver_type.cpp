#include "bdd_from_bits/restrict_resolver_type.hpp"


#include "bdd_from_bits/restrict_operation_type.hpp"


restrict_resolver_type::restrict_resolver_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : resolver_type(bdd, ct) {
    OP = new restrict_operation_type(BDD, ct, 0);
}

restrict_resolver_type::~restrict_resolver_type() {
    delete OP; 
}

unsigned restrict_resolver_type::invoke_specific(unsigned int root) {
    OP->set_levels(topLevel(), botLevel());
    return OP->apply(root);
}

resolver_type * get_restrict_multiterminal_resolver(bdd_forest_type & BDD, compute_table_type & CT) {
    return new restrict_resolver_type(BDD, CT);
}
