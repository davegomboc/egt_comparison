#include "bdd_from_bits/bdd_forest_type.hpp"

#include <cstdlib>
#include <iomanip>

#include "bdd_from_bits/compute_table_type.hpp"
#include "bdd_from_bits/exception_neutrality.hpp"


// #define EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_UNION
// #define EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_ENLARGE


using std::malloc;

const unsigned int INITIAL_SIZE = 1000 * 1000;
const unsigned int DOUBLE_SIZE  = 1024 * 1000 * 1000;


bdd_forest_type::bdd_forest_type(unsigned short nvars, unsigned int * m, unsigned char terminals) {
    num_vars = nvars;
    num_terminals = terminals;
    msround = 1;

    free_nodes = 0;
    max_nodes = INITIAL_SIZE;
    node_array = static_cast<bdd_node_type *>(malloc(max_nodes * sizeof(bdd_node_type)));
    if (nullptr == node_array) bdd_terminate("Malloc fail");
    bdd_node_type zero;
    zero.level = 0;
    zero.child[0] = 0;
    zero.child[1] = 0;
    for (next_node = 0; next_node < terminals; next_node++) {
        // set up terminal nodes to sane values
        node_array[next_node].copy(zero);
        node_array[next_node].next = 0;
    }
    max_next_node = next_node;

    ut_size = max_nodes;
    UT = static_cast <unsigned*> (malloc(ut_size * sizeof(unsigned)));
    if (nullptr == UT) bdd_terminate("Malloc fail");
    for (unsigned i=0; i<ut_size; ++i) {
        UT[i] = 0;
    }
    ut_elements = 0;

    // parent_next[0] = 0;
    // parent_next[1] = 0;

    unary_cache = nullptr;

    CT = nullptr;

    map = m;
   map[0] = 0;
}

bdd_forest_type::~bdd_forest_type() {
    free(node_array);
    free(UT);
    delete[] map;
}

unsigned bdd_forest_type::add_node(bdd_node_type const & N) {
    // Sanity checks
    assert(node_level(N.child[0]) < N.level);
    assert(node_level(N.child[1]) < N.level);

    if (N.child[0] == N.child[1]) return N.child[0];

    unsigned const nh = N.hash();
    for (unsigned int curr = UT[nh % ut_size]; curr; curr = node_array[curr].next) {
        assert(curr < next_node);
        if (node_array[curr].is_duplicate(N)) return curr;
    }

    // No duplicates; make a new node and add it to the UT

    unsigned int ptr = get_unused_handle();
    /*
      ^^ this could change ut_size
    */
    unsigned const H = nh % ut_size;
    node_array[ptr].copy(N);
    node_array[ptr].next = UT[H];
    node_array[ptr].sequence = msround;
    ++ut_elements;
    return (UT[H] = ptr);
}

unsigned bdd_forest_type::mark_and_sweep(unsigned int * roots, unsigned len) {
    ++msround;
    for (unsigned u=0; u<len; u++) {
        mark(roots[u]);
    }

    //
    // Everything marked.
    // Start sweep phase.
    //

    //
    // Sweep compute table, if any
    //
    if (CT) CT->remove_stales(msround);

    //
    // Sweep nodes and rebuild the free list in order of handle :)
    //
    free_nodes = 0;
    unsigned int marked = 0;
    for (unsigned int u = next_node-1; u >= num_terminals; u--) {
        if (0 == node_array[u].level) {
            // already deleted
            recycle(u);
            continue;
        }
        if (msround == node_array[u].sequence) {
            // This node is in use
            ++marked;
            continue;
        }
        // Node is unmarked, delete it
        hash_remove(u);
        recycle(u);
    }

    return marked;
}

unsigned bdd_forest_type::count_pattern(unsigned int * low, unsigned int * high) const {
    unsigned int total = 0;
    for (unsigned int t = 0; t < num_terminals; t++) {
        if (low) low[t] = 0;
        if (high) high[t] = 0;
    }
    for (unsigned h = num_terminals; h < next_node; h++) {
        if (node_array[h].level) {
            ++total;
            if (low && node_array[h].child[0] < num_terminals) {
                low[node_array[h].child[0]]++;
            }
            if (high && node_array[h].child[1] < num_terminals) {
                high[node_array[h].child[1]]++;
            }
        }
    }
    return total;
}

void bdd_forest_type::display(std::ostream & out) const {
    // Not the fastest, but it works
    for (unsigned short L = num_vars; L > 0; L--) {
        out << "Level " << L << " nodes:\n";
        out << "------------------------\n";

        for (unsigned int h = num_terminals; h < next_node; h++) if (node_array[h].level == L) {
            out << "  # " << std::setw(9) << h;
            for (unsigned int i = 0; i < 2; i++) {
                out << "  " << i << "child: ";
                out << std::setw(9) << node_array[h].child[i];
                if (node_array[h].child[i] < num_terminals) {
                    out << "T";
                } else {
                    out << "n";
                }
            }
            out << "\n";
        }
    }
}

void bdd_forest_type::write_exch(std::ostream & out, unsigned int * rts, unsigned int nr) const {
    // nstat will be 0 if the node is not reachable, 1+level otherwise
    unsigned char * nstat = new unsigned char[next_node+1];
    for (unsigned i = 0; i < num_terminals; i++) nstat[i] = 1;
    for (unsigned i = num_terminals; i <= next_node; i++) nstat[i] = 0;

    // nq is the queue of nodes to explore
    unsigned * nq = new unsigned[next_node+1];
    unsigned nqhead = 0;
    for (unsigned i = 0; i < nr; i++) {
        unsigned const e = rts[i];
        if (nstat[e]) continue;
        nstat[e] = static_cast<unsigned char>(1 + node_level(e));
        nq[nqhead++] = e;
    }
    // BFS of the bdd
    unsigned int num_nonterms = 0;
    for (unsigned int nqtail = 0; nqtail < nqhead; nqtail++) {
        unsigned int const e = nq[nqtail];
        if (nstat[e] < 2) continue; // don't need to visit terminals
        num_nonterms++;
        unsigned int const e0 = node_child0(e);
        unsigned int const e1 = node_child1(e);
        if (0 == nstat[e0]) {
            nstat[e0] = static_cast<unsigned char>(1 + node_level(e0));
            nq[nqhead++] = e0;
        }
        if (0 == nstat[e1]) {
            nstat[e1] = static_cast<unsigned char>(1 + node_level(e1));
            nq[nqhead++] = e1;
        }
    }

    out << "BDD\n";
    out << "L " << num_vars << "\n";
    out << "T " << unsigned(num_terminals) << "\n";
    out << "N " << num_nonterms << "\n";
    out << "R " << nr << "\n";

    for (unsigned int i = 0; i < num_terminals; i++) nq[i] = i;
    for (unsigned int i = num_terminals; i <= next_node; i++) nq[i] = 0;

    unsigned int fnum = num_terminals;

    //
    // Show nodes by level
    //
    for (short k = 1; k <= num_vars; k++) {
        out << "; level " << k << "\n";
        unsigned int cnt = 0;
        for (unsigned int i = 0; i <= next_node; i++) {
            if (k + 1 == nstat[i]) {
                cnt++;
            }
        }
        out << "n " << cnt << "\n";
        for (unsigned int i = 0; i <= next_node; i++) {
            if (k + 1 == nstat[i]) {
                out << nq[node_child0(i)] << "\t" << nq[node_child1(i)];
               out << "\t; " << fnum << "\n";
                nq[i] = fnum++;
            }
        }
    }

    //
    // Show the roots
    //
    out << "; roots\n";
    for (unsigned int i = 0; i < nr; i++) {
        out << "r " << nq[rts[i]] << "\n";
    }
}

/*

  Private helpers below here

*/

void bdd_forest_type::mark(unsigned int root) {
    if (root < num_terminals) return;
    if (node_array[root].sequence == msround) return;
    node_array[root].sequence = msround;
    mark(node_array[root].child[0]);
    mark(node_array[root].child[1]);
}

unsigned int bdd_forest_type::get_unused_handle() {
    if (free_nodes) {
        unsigned int curr = free_nodes;
        free_nodes = node_array[free_nodes].next;
        return curr;
    }

    if (next_node + 1 >= max_nodes) {
        enlarge_node_array();
    }
    if (next_node == max_next_node) {
        max_next_node++;
    }
    return next_node++;
}

void bdd_forest_type::enlarge_node_array() {
    unsigned const old_max = max_nodes;
    if (max_nodes < DOUBLE_SIZE) {
        max_nodes *= 2;
    } else {
        if (max_nodes < 4000000000) {
            max_nodes += DOUBLE_SIZE;
        }
    }
#ifdef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_ENLARGE
    fprintf(stderr, "Enlarging node array and UT: %u. ", max_nodes);
#endif

    if (old_max == max_nodes) bdd_terminate("Too many nodes");
    node_array = static_cast<bdd_node_type *>(realloc(node_array, max_nodes * sizeof(bdd_node_type)));
    if (nullptr == node_array) bdd_terminate("Realloc fail");

    // Also enlarge hash table
    ut_size = max_nodes;
    UT = static_cast<unsigned int *>(realloc(UT, ut_size * sizeof(unsigned int)));
    if (nullptr == UT) bdd_terminate("Realloc fail");
    for (unsigned int i = 0; i < ut_size; ++i) {
        UT[i] = 0;
    }

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_ENLARGE
    fprintf(stderr, "Rehashing.");
#endif

    // Re-hash all nodes
    for (unsigned int i = 0; i < next_node; ++i) {
        bdd_node_type const & N = get_node(i);
        if (N.level <= 0) continue;   // terminal, or deleted

        unsigned H = N.hash();
        H %= ut_size;
        node_array[i].next = UT[H];
        UT[H] = i;
    }

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_ENLARGE
    fprintf(stderr, "  Done.\n");
#endif
}

void bdd_forest_type::hash_remove(unsigned int handle) {
    unsigned int H = get_node(handle).hash();
    H %= ut_size;

    unsigned int prev = 0;
    for (unsigned int curr = UT[H]; curr; curr = node_array[curr].next) {
        if (curr == handle) {
            if (prev) {
                node_array[prev].next = node_array[handle].next;
            } else {
                UT[H] = node_array[handle].next;
            }
            --ut_elements;
            return;
        }
        prev = curr;
    }

    // Not found?
}


unsigned bdd_forest_type::union_rec(unsigned short K, unsigned root, char** minterm, unsigned N) {
    if (0 == N) return root;

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_UNION
    std::cerr << "Inside union_rec level " << K << ", root " << root << "\n";
#endif

    if (0 == K) {
        unsigned int max = root;
        for (unsigned int i = 0; i < N; i++) {
            unsigned int m = char2terminal(minterm[i][0]);
            if (m > max) max = m;
        }
#ifdef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_UNION
        std::cerr << "max is " << max << " (returning)\n";
#endif
        return max;
    }

    // Two-finger algorithm to sort 0,1 values in position K
    unsigned int left = 0;
    unsigned int right = N - 1;

    for (;;) {
        // move left to first 1 value
        for ( ; left < right; left++) {
            if ('1' == minterm[left][K]) break;
        }
        // move right to first 0 value
        for ( ; left < right; right--) {
            if ('0' == minterm[right][K]) break;
        }
        // Stop?
        if (left >= right) break;
        // we have a 1 before a 0, swap them
        char * tmp = minterm[left];
        minterm[left] = minterm[right];
        minterm[right] = tmp;
        // For sure we can move them one spot
        ++left;
        --right;
    } // loop

#ifdef EGT_COMPARISON_BDD_FROM_BITS_BDD_FOREST_TYPE_DEBUG_UNION
    std::cerr << "Ordered minterms:\n";
    for (unsigned int i = 0; i < N; i++) {
        std::cerr << "    " << i << ": ";
        for (short j = 0; j <= K; j++) {
            std::cerr << minterm[i][j];
        }
        std::cerr << "\n";
    }
    std::cerr << "Left is " << left << "\n";
#endif

    if (left < N) {
        if ('0' == minterm[left][K]) left++;
        if (left < N) assert('1' == minterm[left][K]);
        if (left > 0) assert('0' == minterm[left-1][K]);
    }
    //
    // elements 0 .. left-1 are 0's, left .. N-1 are 1's.
    //

    //
    // Determine down pointers, taking care of redundant nodes
    //
    unsigned int down[2];
    if (node_level(root) < K) {
        down[0] = down[1] = root;
    } else {
        down[0] = node_child0(root);
        down[1] = node_child1(root);
    }

    //
    // Build new node
    //
    bdd_node_type tmp;
    tmp.level = K;
    tmp.child[0] = union_rec(K-1, down[0], minterm, left);
    tmp.child[1] = union_rec(K-1, down[1], minterm + left, N-left);

    return add_node(tmp);
}


void bdd_forest_type::minterms_rec(std::ostream &out, unsigned root, bool show_zeroes) const {
    if (root < num_terminals) {
        if (show_zeroes || root) {
            minterm2varray(_minterm, _varray);
            out << _varray << "    " << root << "\n";
        }
        return;
    }

    for (unsigned i = 0; i < 2; i++) {
        short v;
        for (v = 1; v < node_array[root].level; v++) _minterm[v] = '-';
        _minterm[v] = static_cast <char>('0' + i);
        minterms_rec(out, node_array[root].child[i], show_zeroes);
    }
}
