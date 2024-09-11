#include "bdd_from_bits/restrict_operation_type.hpp"


// #define EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_TRACE


restrict_operation_type::restrict_operation_type(
    bdd_forest_type & bdd,
    compute_table_type & ct,
    unsigned int UNK
) : bdd_operation_type(bdd, ct, "restrict_mt"), UNKNOWN(UNK) {
    set_unary_op();
    set_answer_node();
    set_levels(bdd.get_num_vars(), 0);
}

unsigned int restrict_operation_type::apply(unsigned int root) {
    if (is_terminal(root)) return root;

    if (node_level(root) < botL) return root;

    unsigned ans;
    if (has_sUCT_entry(root, sequence, ans)) return ans;

    if (node_level(root) <= topL) {
        if (node_child0(root) == UNKNOWN) {
#ifdef EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_TRACE
            ans = apply(node_child1(root));
            printf("apply(%u) : apply(%u, %u, %u) : %u\n", root,
            node_level(root), node_child0(root), node_child1(root), ans);
            return ans;
#else
            return add_sUCT_entry(root, sequence, apply(node_child1(root)));
#endif
        }

        if (node_child1(root) == UNKNOWN) {
#ifdef EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_TRACE
            ans = apply(node_child0(root));
            printf("apply(%u) : apply(%u, %u, %u) : %u\n", root,
            node_level(root), node_child0(root), node_child1(root), ans);
            return ans;
#else
            return add_sUCT_entry(root, sequence, apply(node_child0(root)));
#endif
        }
    }

    bdd_node_type tmp;
    tmp.level = node_level(root);
    tmp.child[0] = apply(node_child0(root));
    tmp.child[1] = apply(node_child1(root));
#ifdef EGT_COMPARISON_BDD_FROM_BITS_RESTRICT_OPERATION_TYPE_TRACE
    ans = add_node(tmp);
    printf("apply(%u) : apply((%u, %u, %u) : (%u, %u, %u) : %u \n", root,
    node_level(root), node_child0(root), node_child1(root), tmp.level, tmp.child[0], tmp.child[1], ans);

    return add_sUCT_entry(root, sequence, ans);
#else
    return add_sUCT_entry(root, sequence, add_node(tmp));
#endif
}
