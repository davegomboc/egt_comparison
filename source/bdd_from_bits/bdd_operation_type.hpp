#ifndef EGT_COMPARISON_BDD_FROM_BITS_BDD_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_BDD_OPERATION_TYPE_HPP


#include <cassert>

#include "bdd_from_bits/bdd_forest_type.hpp"
#include "bdd_from_bits/compute_table_type.hpp"


class bdd_operation_type {
    char const * name;
    compute_table_type & CT;
    bdd_forest_type & BDD;
    bool answer_is_node;
    bool binary_op;
    unsigned char OPCODE;

public:
    bdd_operation_type(bdd_forest_type & bdd, compute_table_type & ct, char const * n);
    ~bdd_operation_type();

    inline char const * get_name() const { return name; }
    inline unsigned char get_opcode() const { return OPCODE; }

    inline bool entry_is_stale(unsigned short seqno, unsigned int P, unsigned int Q, unsigned int ans) const {
        if (BDD.get_node(P).sequence != seqno) return true;
        if (binary_op) {
            if (BDD.get_node(Q).sequence != seqno) return true;
        }
        if (answer_is_node) {
            return BDD.get_node(ans).sequence != seqno;
        } else {
            return false;
        }
    }

protected:
    void set_answer_node() {
        answer_is_node = true;
    }
    void set_answer_other() {
        answer_is_node = false;
    }
    void set_binary_op() {
        binary_op = true;
    }
    void set_unary_op() {
        binary_op = false;
    }

    inline unsigned int add_node(bdd_node_type const & N)
    {
        return BDD.add_node(N);
    }

    inline unsigned short node_level(unsigned int handle) const
    {
        return BDD.node_level(handle);
    }

    inline unsigned int node_child0(unsigned int handle) const
    {
        return BDD.node_child0(handle);
    }

    inline unsigned int node_child1(unsigned int handle) const
    {
        return BDD.node_child1(handle);
    }

    inline bool is_terminal(unsigned int P) const
    {
        return P < BDD.get_num_terminals();
    }

    inline bool has_BCT_entry(unsigned int P, unsigned int Q, unsigned int & ans) const {
        assert(binary_op);
        return CT.find_binary(OPCODE, P, Q, ans);
    }

    inline unsigned add_BCT_entry(unsigned int P, unsigned int Q, unsigned int ans) {
        assert(binary_op);
        CT.add_binary(OPCODE, P, Q, ans);
        return ans;
    }

    inline bool has_UCT_entry(unsigned int P, unsigned int & ans) const {
        assert(!binary_op);
        return CT.find_unary(OPCODE, P, ans);
    }

    inline unsigned add_UCT_entry(unsigned int P, unsigned int ans) {
        assert(!binary_op);
        CT.add_unary(OPCODE, P, ans);
        return ans;
    }

    inline bool has_sUCT_entry(unsigned int P, unsigned int seq, unsigned int & ans) const {
        assert(!binary_op);
        return CT.find_binary(OPCODE, P, seq, ans);
    }

    inline unsigned add_sUCT_entry(unsigned int P, unsigned int seq, unsigned int ans) const {
        assert(!binary_op);
        CT.add_binary(OPCODE, P, seq, ans);
        return ans;
    }
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_BDD_OPERATION_TYPE_HPP
