#include "bdd_from_bits/one_sided_matching_operation_type.hpp"


#include <cassert>


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


one_sided_matching_operation_type::one_sided_matching_operation_type(
    bdd_forest_type & bdd,
    compute_table_type & ct,
    compare_operation_type & comp
) : bdd_operation_type(bdd, ct, "osm_mt"), COMP(comp) {
    set_unary_op();
    set_answer_node();
}

unsigned int one_sided_matching_operation_type::apply(unsigned int root) {
    if (is_terminal(root)) return root;

    if (node_level(root) < botL) return root;

    unsigned int ans;
    if (has_sUCT_entry(root, sequence, ans)) return ans;

    if (node_level(root) <= topL) {
        unsigned char child_cmp = COMP.apply(node_child0(root), node_child1(root));
        if ('>' == child_cmp) {
            // child 0 -> child 1
            return add_sUCT_entry(root, sequence, apply(node_child1(root)));
        }
        if ('<' == child_cmp) {
            // child 1 -> child 0
            return add_sUCT_entry(root, sequence, apply(node_child0(root)));
        }
        // should be impossible
        assert('=' != child_cmp);
    }

    bdd_node_type tmp;
    tmp.level = node_level(root);
    tmp.child[0] = apply(node_child0(root));
    tmp.child[1] = apply(node_child1(root));

    return add_sUCT_entry(root, sequence, add_node(tmp));
}
