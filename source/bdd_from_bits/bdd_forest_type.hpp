#ifndef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_HPP


#include <cassert>
#include <iostream>

#include "bdd_from_bits/bdd_node_type.hpp"


class compute_table_type;

class bdd_forest_type {
public:
    /**
      Initialize a forest with the given number of variables and terminals.
      Terminal values are 0, 1, ..., terminals-1.
      The map array specifies the level for each input variable.
      I.e., map[1] gives the level for variable 1, map[2] gives
      the level for variable 2, etc.  map[0] will be set to 0.
    */
    bdd_forest_type(unsigned int short nvars, unsigned int * map, unsigned char terminals = 2);

    /**
      Destroy the forest
    */
    ~bdd_forest_type();

    inline unsigned int peak_nodes() const {
        return max_next_node - num_terminals;
    }

    inline unsigned short get_num_vars() const {
        return num_vars;
    }

    inline unsigned int get_num_terminals() const {
        return num_terminals;
    }

    static inline unsigned int char2terminal(char t) {
        return static_cast<unsigned int>(t - '0');
    }

    static inline char terminal2char(unsigned int v) {
        return char('0' + v);
    }

    /**
      Convert from variable array to level array.
        @param  input   input[k] is the value for variable k+1.
        @param  output  output[k] is the value for level k.
    */
    inline void varray2minterm(char const * input, char * output) const {
        for (short b = 0; b < num_vars; b++) {
            output[map[b+1]] = input[b];
        }
    }

    /**
      Convert from level array to variable array.
        @param  input   input[k] is the value for level k.
        @param  output  output[k] is the value for variable k+1.
    */
    inline void minterm2varray(char const * input, char * output) const {
        for (short b = 0; b < num_vars; b++) {
            output[b] = input[map[b+1]];
        }
    }


    /**
      Evaluate a function for given variable assignments, as a minterm.
    */
    inline char evaluate(unsigned int root, char const * minterm) const {
        using namespace std;
        for (;;) {
            if (root < get_num_terminals()) {
                return terminal2char(root);
            }
            bdd_node_type const & node = node_array[root];
            char const var = minterm[node.level];
            assert('0' == var || '1' == var);
            root = node.child[var - '0'];
        }
    }

    /**
      Find or add a node in the BDD.
    */
    unsigned int add_node(bdd_node_type const & N);

    /**
      Given a node handle, return the node struct.
    */
    inline bdd_node_type const & get_node(unsigned int handle) const {
        assert(handle < next_node);
        return node_array[handle];
    }

    /**
      Given a node handle, return the node's level.
    */
    inline unsigned short node_level(unsigned int handle) const {
        assert(handle < next_node);
        return node_array[handle].level;
    }

    /**
      Given a node handle, return the node's low child.
    */
    inline unsigned int node_child0(unsigned int handle) const {
        assert(handle < next_node);
        return node_array[handle].child[0];
    }

    /**
      Given a node handle, return the node's high child.
    */
    inline unsigned int node_child1(unsigned int handle) const {
        assert(handle < next_node);
        return node_array[handle].child[1];
    }


    /**
      Garbage collection.  Keeps the root node and all terminals;
      anything unreachable is recycled.
      Returns the number of reachable non-terminal nodes, including roots.
    */
    unsigned int mark_and_sweep(unsigned int * roots, unsigned int len);

    inline unsigned int mark_and_sweep(unsigned int root) {
        return mark_and_sweep(&root, 1);
    }

    /**
      Build a new BDD handle as the union of the given root,
      and a list of minterms.  Each minterm is a string of length
      number of levels + 1, with minterm[0] as the terminal value,
      and minterm[i] the value at level i, as characters
      (use char2terminal, terminal2char methods for terminals).

      TBD: allow '?' for don't care variables.

      The minterm array WILL be modified: the minterms will be
      reordered to optimize the union operation.
    */
    inline unsigned int union_minterms(unsigned int root, char ** minterm, unsigned int N) {
        return union_rec(num_vars, root, minterm, N);
    }

    /**
      Count nodes with a low or high edge directly to a terminal node.
      returns the total number of nodes.
    */
    unsigned int count_pattern(unsigned int * low, unsigned int * high) const;

    /**
      Display the entire forest, by levels.
    */
    void display(std::ostream & out) const;

    /**
      Write the given BDDs to a file.
    */
    void write_exch(std::ostream & out, unsigned int * roots, unsigned int len) const;

    inline void write_exch(std::ostream & out, unsigned int root) const {
        write_exch(out, &root, 1);
    }

    /**
      Display minterms for the given function (root node).
        @param  out   Stream to send output to
        @param  root  Root node of the function
        @param  show_zeroes		Show minterms that lead to value 0
    */
    inline void show_minterms(std::ostream & out, unsigned int root, bool show_zeroes = false) {
        _minterm = new char[num_vars+2];
        _varray = new char[num_vars+1];
        _varray[num_vars] = 0;
        for (short i=0; i<=num_vars; i++) _minterm[i] = '-';
        minterms_rec(out, root, show_zeroes);
        delete[] _varray;
        delete[] _minterm;
        _varray = nullptr;
        _minterm = nullptr;
    }

    // void build_parent_lists(bool show);

    inline void set_CT(compute_table_type * ct) {
        CT = ct;
    }

private:
    inline void recycle(unsigned int handle) {
        assert(handle >= num_terminals);
        if (next_node - 1 == handle) {
            --next_node;
        } else {
            node_array[handle].recycle(free_nodes);
            free_nodes = handle;
        }
    }

    void mark(unsigned int root);
    unsigned int get_unused_handle();
    void enlarge_node_array();
    void hash_remove(unsigned int handle);

    unsigned int union_rec(unsigned short level, unsigned int root, char ** minterm, unsigned int N);

    void minterms_rec(std::ostream & out, unsigned int root, bool show_zeroes) const;

    // unsigned int redundant_rec(unsigned int root);

    unsigned short num_vars;
    unsigned char num_terminals;
    unsigned short msround;

    bdd_node_type * node_array;
    unsigned int next_node;
    unsigned int max_next_node;
    unsigned int max_nodes;
    unsigned int free_nodes;

    unsigned int * UT;
    unsigned int ut_size;
    unsigned int ut_elements;

    unsigned int * unary_cache;

    compute_table_type * CT;  // used to synch garbage collection

    unsigned int * map;  // variable to level mapping.

    // For minterms_rec
    char * _minterm;
    char * _varray;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_HPP
