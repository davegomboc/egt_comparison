#include "bdd_from_bits/resolver_type.hpp"


resolver_type::resolver_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : BDD(bdd), CT(ct) {
    toplevel = 0;
    botlevel = 0;
    invoke_count = 0;
}

resolver_type::~resolver_type() {}
