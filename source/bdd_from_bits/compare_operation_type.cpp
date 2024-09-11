#include "bdd_from_bits/compare_operation_type.hpp"


compare_operation_type::compare_operation_type(
    bdd_forest_type & bdd,
    compute_table_type & ct
) : bdd_operation_type(bdd, ct, "compare") {
    set_answer_other();
    set_binary_op();
}

unsigned char compare_operation_type::apply(unsigned int P, unsigned int Q) {
    if (P == Q) return '=';
    if (P == 0) return '>';
    if (Q == 0) return '<';
    if (is_terminal(P) && is_terminal(Q)) return '!';

    bool swap = false;
    if (P > Q) {
        // Deal with 'almost' commutativity
        unsigned T = P;
        P = Q;
        Q = T;
        swap = true;
    }
    unsigned int answer;

    if (has_BCT_entry(P, Q, answer)) {
        return flip_comparison(swap, static_cast<unsigned char>(answer));
    }

    unsigned char cmp0, cmp1;
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

    if ( ('=' == cmp0) || (cmp0 == cmp1) ) {
        answer = cmp1;
    } else {
        answer = '!';
    }

    return flip_comparison(swap, static_cast<unsigned char>(add_BCT_entry(P, Q, answer)));
}
