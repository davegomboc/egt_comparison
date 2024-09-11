#include "bdd_from_bits/one_sided_matching_resolver_type.hpp"


one_sided_matching_resolver_type::one_sided_matching_resolver_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : resolver_type(bdd, ct) {
    comp = new compare_operation_type(BDD, ct);
    OP = new one_sided_matching_operation_type(BDD, ct, *comp);
}

one_sided_matching_resolver_type::~one_sided_matching_resolver_type() {
    delete OP;
    delete comp;
}

unsigned int one_sided_matching_resolver_type::invoke_specific(unsigned int root) {
    OP->set_levels(topLevel(), botLevel());
    return OP->apply(root);
}

resolver_type * get_one_sided_matching_multiterminal_resolver(bdd_forest_type & BDD, compute_table_type & CT) {
    return new one_sided_matching_resolver_type(BDD, CT);
}
