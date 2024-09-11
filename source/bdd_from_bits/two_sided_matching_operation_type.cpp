#include "bdd_from_bits/two_sided_matching_operation_type.hpp"


two_sided_matching_operation_type::two_sided_matching_operation_type(
    bdd_forest_type & bdd,
    compute_table_type & ct,
    has_common_operation_type & comp,
    common_operation_type & common
) : bdd_operation_type(bdd, ct, "tsm_mt"), COMP(comp), COMMON(common) {
    set_unary_op();
    set_answer_node();
}

unsigned int two_sided_matching_operation_type::apply(unsigned int root) {
    if (is_terminal(root)) return root;
    if (node_level(root) < botL) return root;

    unsigned int ans;
    if (has_sUCT_entry(root, sequence, ans)) return ans;

    if (COMP.apply(node_child0(root), node_child1(root))) {
        return add_sUCT_entry(root, sequence, apply(COMMON.apply(node_child0(root), node_child1(root))));
    }

    bdd_node_type tmp;
    tmp.level = node_level(root);
    tmp.child[0] = apply(node_child0(root));
    tmp.child[1] = apply(node_child1(root));

    return add_sUCT_entry(root, sequence, add_node(tmp));
}

// See clang++ -Wweak-vtables diagnostic documentation for why this is here.
void two_sided_matching_operation_type::vtable_pinning_method() const {}
