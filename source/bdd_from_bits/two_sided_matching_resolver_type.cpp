#include "bdd_from_bits/two_sided_matching_resolver_type.hpp"


two_sided_matching_resolver_type::two_sided_matching_resolver_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : resolver_type(bdd, ct) {
    comp = new has_common_operation_type(BDD, ct);
    common = new common_operation_type(BDD, ct);
    OP = new two_sided_matching_operation_type(BDD, ct, *comp, *common);
}

two_sided_matching_resolver_type::~two_sided_matching_resolver_type() {
    delete OP; 
    delete common;
    delete comp;
}

unsigned int two_sided_matching_resolver_type::invoke_specific(unsigned int root) {
    OP->set_levels(topLevel(), botLevel());
    return OP->apply(root);
}

resolver_type * get_two_sided_matching_multiterminal_resolver(bdd_forest_type & BDD, compute_table_type & CT) {
    return new two_sided_matching_resolver_type(BDD, CT);
}
