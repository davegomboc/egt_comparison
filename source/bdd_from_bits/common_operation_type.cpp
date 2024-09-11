#include "bdd_from_bits/common_operation_type.hpp"


common_operation_type::common_operation_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : bdd_operation_type(bdd, ct, "common") {
    set_answer_node();
    set_binary_op();
}

unsigned int common_operation_type::apply(unsigned P, unsigned Q) {
    if (P == Q) return P;
    if (P == 0) return Q;
    if (Q == 0) return P;
    if (is_terminal(P) && is_terminal(Q)) return 0;   

    if (P > Q) {
        unsigned int T = P;
        P = Q;
        Q = T;
    } 
    unsigned int answer;

    if (has_BCT_entry(P, Q, answer)) {
        return answer;
    }

    bdd_node_type tmp;
    if (node_level(P) < node_level(Q)) {
        tmp.level = node_level(Q);
        tmp.child[0] = apply(P, node_child0(Q));
        tmp.child[1] = apply(P, node_child1(Q));
    } else if (node_level(P) > node_level(Q)) {
        tmp.level = node_level(P);
        tmp.child[0] = apply(node_child0(P), Q);
        tmp.child[1] = apply(node_child1(P), Q);
    } else {
        tmp.level = node_level(P);
        tmp.child[0] = apply(node_child0(P), node_child0(Q));
        tmp.child[1] = apply(node_child1(P), node_child1(Q));
    }

    return add_BCT_entry(P, Q, add_node(tmp));
}
