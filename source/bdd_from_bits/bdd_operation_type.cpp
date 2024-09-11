#include "bdd_operation_type.hpp"


bdd_operation_type::bdd_operation_type(bdd_forest_type & bdd, compute_table_type & ct, char const * n)
  : name(n), CT(ct), BDD(bdd) {
    answer_is_node = true;
    OPCODE = CT.register_op(this);
}

bdd_operation_type::~bdd_operation_type() {
    CT.unregister_op(this);
}
