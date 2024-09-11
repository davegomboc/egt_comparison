#include "bdd_from_bits/has_common_operation_type.hpp"

//
// Checks if two nodes can be concretized to each other.
// That means they are equal except possibly for don't cares.
//

has_common_operation_type::has_common_operation_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : bdd_operation_type(bdd, ct, "has_common") {
    set_answer_other();
    set_binary_op();
}

bool has_common_operation_type::apply(unsigned int P, unsigned int Q) {
    if (P == Q) return true;
    if (P == 0) return true;
    if (Q == 0) return true;
    if (is_terminal(P) && is_terminal(Q)) return false;

    if (P > Q) {
        unsigned int T = P;
        P = Q;
        Q = T;
    } 
    unsigned int answer;

    if (has_BCT_entry(P, Q, answer)) {
        return answer;
    }

    bool cmp0, cmp1;
    if (node_level(P) < node_level(Q)) {
        cmp0 = apply(P, node_child0(Q));
        cmp1 = apply(P, node_child1(Q));
    } else if (node_level(P) > node_level(Q)) {
        cmp0 = apply(node_child0(P), Q);
        cmp1 = apply(node_child1(P), Q);
    } else {
        cmp0 = apply(node_child0(P), node_child0(Q));
        cmp1 = apply(node_child1(P), node_child1(Q));
    }

    answer = (cmp0 && cmp1) ? 1 : 0;

    return add_BCT_entry(P, Q, answer);
}
