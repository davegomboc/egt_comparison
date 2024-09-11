#include "bdd_from_bits/fixed_decision_diagram_type.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <sstream>

#include "bdd_from_bits/exception_neutrality.hpp"


// #define EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_DEBUG_PARSER

fixed_decision_diagram_type & fixed_decision_diagram_type::operator=(
    fixed_decision_diagram_type && other
) {
    levels = other.levels;
    terminals = other.terminals;
    next_node = other.next_node;
    max_nodes = other.max_nodes;
    file_nodes = other.file_nodes;
    node_level = other.node_level;
    other.node_level = nullptr;
    node_child0 = other.node_child0;
    other.node_child0 = nullptr;
    node_child1 = other.node_child1;
    other.node_child1 = nullptr;
    unique_table = other.unique_table;
    other.unique_table = nullptr;
    unique_table_next = other.unique_table_next;
    other.unique_table_next = nullptr;
    return *this;
}

fixed_decision_diagram_type::fixed_decision_diagram_type(
    fixed_decision_diagram_type && other
) {
    levels = other.levels;
    terminals = other.terminals;
    next_node = other.next_node;
    max_nodes = other.max_nodes;
    file_nodes = other.file_nodes;
    node_level = other.node_level;
    other.node_level = nullptr;
    node_child0 = other.node_child0;
    other.node_child0 = nullptr;
    node_child1 = other.node_child1;
    other.node_child1 = nullptr;
    unique_table = other.unique_table;
    other.unique_table = nullptr;
    unique_table_next = other.unique_table_next;
    other.unique_table_next = nullptr;
}

fixed_decision_diagram_type::fixed_decision_diagram_type() {
    levels = 0;
    terminals = 0;
    next_node = 0;

    file_nodes = 0;

    max_nodes = 0;
    node_level = nullptr;
    node_child0 = nullptr;
    node_child1 = nullptr;
    unique_table = nullptr;
    unique_table_next = nullptr;
}

fixed_decision_diagram_type::~fixed_decision_diagram_type() {
    std::free(node_level);
    node_level = nullptr;
    std::free(node_child0);
    node_child0 = nullptr;
    std::free(node_child1);
    node_child1 = nullptr;

    std::free(unique_table);
    unique_table = nullptr;
    std::free(unique_table_next);
    unique_table_next = nullptr;
}

char skip_whitespace(FILE * file_in) {
    char comment = 0;
    for (;;) {
        int c = fgetc(file_in);
        if (EOF == c) {
            bdd_terminate("EE: Unexpectedly encountered the end of the file.");
        }
        if ('\n' == c) {
            comment = 0;
            continue;
        }
        if (';' == c) {
            comment = 1;
            continue;
        }
        if (comment) continue;
        if (' ' == c) continue;
        if ('\t' == c) continue;
        if ('\r' == c) continue;

        return static_cast<char>(c);
    }
}

inline void expect_char(FILE * const file_in, char const next) {
    char const ch{skip_whitespace(file_in)};
    if (ch != next) {
        std::ostringstream error_message{"Expected character '"};
        error_message << next << "', but found '" << ch << " instead.";
        bdd_terminate(error_message.str());
    }
}

inline unsigned int expect_unsigned(FILE * const file_in) {
    char const ch = skip_whitespace(file_in);
    if ((ch > '9') || (ch < '0')) {
        std::ostringstream error_message{"EE: Expected integer, but found '"};
        error_message << ch << "' instead.";
        bdd_terminate(error_message.str());
    }
    std::ungetc(ch, file_in);
    unsigned int u;
    std::fscanf(file_in, "%u", &u);
    return u;
}

unsigned int fixed_decision_diagram_type::add_nodes_from_file(
    char const * pathname
) {
    if (nullptr != node_level && nullptr == unique_table) {
        bdd_terminate("EE: Can't add nodes after calling done_building().");
    }

    //
    // Determine extension, and open file/pipeline for reading
    //
    char const * ext{};
    for (char const * ptr = pathname; *ptr; ptr++) {
        if ('.' == *ptr) ext = ptr+1;
    }
    char const * zcat{};
    if (ext) {
        if (0 == std::strcmp("xz", ext)) {
            zcat = "xzcat";
        } else if (0 == std::strcmp("gz", ext)) {
            zcat = "zcat";
        } else if (0 == std::strcmp("bz2", ext)) {
            zcat = "bzcat";
        }
    }
    FILE * file_in{};
    if (zcat) {
        char cmd[1024];
        std::snprintf(cmd, 1024, "%s %s", zcat, pathname);
        file_in = popen(cmd, "r");
        if (nullptr == file_in) {
            std::ostringstream error_message{"EE: Couldn't popen the path \""};
            error_message << pathname << "\".";
            bdd_terminate(error_message.str());
        }
    } else {
        file_in = std::fopen(pathname, "r");
        if (nullptr == file_in) {
            std::ostringstream error_message{"EE: Couldn't fopen the path \""};
            error_message << pathname << "\".";
            bdd_terminate(error_message.str());
        }
    }

    //
    // Parse input
    //
    char buffer[4];
    fread(buffer, 1, 4, file_in);
    if (('B' != buffer[0]) or ('D' != buffer[1]) or ('D' != buffer[2])
     or ('\n' != buffer[3])) {
        bdd_terminate("EE: Not a BDD file: missing the expected BDD file magic "
         "characters.");
    }

    //
    // Input order L, T, N, R
    //
    expect_char(file_in, 'L');
    unsigned int file_levels = expect_unsigned(file_in);
#ifdef EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_DEBUG_PARSER
    std::printf("Got L %u\n", file_levels);
#endif
    if (0 == levels) {
        assert(file_levels <= std::numeric_limits<decltype(levels)>::max());
        levels = static_cast<decltype(levels)>(file_levels);
    }
    if (levels != file_levels) {
        bdd_terminate("EE: Incompatible number of BDD levels.");
    }

    expect_char(file_in, 'T');
    unsigned int file_terms = expect_unsigned(file_in);
#ifdef EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_DEBUG_PARSER
    std::printf("Got T %u\n", file_terms);
#endif
    if (0 == terminals) {
        terminals = file_terms;
        next_node = file_terms;
    }
    if (file_terms > terminals) {
        bdd_terminate("EE: Incompatible number of BDD terminals.");
    }

    expect_char(file_in, 'N');
    file_nodes = expect_unsigned(file_in);
#ifdef EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_DEBUG_PARSER
    std::printf("Got N %u\n", file_nodes);
#endif
    enlarge_unique_table(file_nodes + next_node);
    unsigned int * file2handle = new unsigned int [file_nodes + file_terms];
    // TODO: Use std::fill.
    for (unsigned int u{}; u < file_terms; ++u) {
        file2handle[u] = u;
    }
    for (unsigned int u{}; u < file_nodes; ++u) {
        file2handle[u + file_terms] = 0xffffffff;
    }

    // TBD - build mapping from file node index to forest index

    expect_char(file_in, 'R');
    unsigned int file_roots = expect_unsigned(file_in);
#ifdef EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_DEBUG_PARSER
    std::printf("Got R %u\n", file_roots);
#endif
    if (file_roots > 1) {
        std::ostringstream error_message{"EE: Only one root was expected, "
         "but "};
        error_message << file_roots << " were found.";
        bdd_terminate(error_message.str());
    }

    //
    // For each level, read nodes
    //
    unsigned int file_index = file_terms;
    unsigned char k;
    for (k = 1; k <= file_levels; ++k) {
        expect_char(file_in, 'n');
        // TODO: Rename 'nk' to something more comprehensible.
        unsigned int nk = expect_unsigned(file_in);
#ifdef EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_DEBUG_PARSER
        std::printf("Reading %u nodes at level %u\n", nk, k);
#endif
        for (; nk; --nk) {
            unsigned int low  = file2handle[expect_unsigned(file_in)];
            unsigned int high = file2handle[expect_unsigned(file_in)];
            assert(low < 0xffffffff);
            assert(high < 0xffffffff);

            unsigned int handle = add_unique(k, low, high);
            file2handle[file_index] = handle;
            ++file_index;
        } // for nk
    } // for k

    //
    // Read roots
    //
    expect_char(file_in, 'r');
    unsigned int root = file2handle[expect_unsigned(file_in)];

    //
    // Cleanup
    //
    if (ext) {
        pclose(file_in);
    } else {
        std::fclose(file_in);
    }
    delete[] file2handle;
    return root;
}

void fixed_decision_diagram_type::done_building() {
    std::free(unique_table);
    unique_table = nullptr;

    std::free(unique_table_next);
    unique_table_next = nullptr;
}

unsigned long fixed_decision_diagram_type::in_use_byte_count() const {
    unsigned long result{nonterminal_node_count()};
    result *= sizeof(unsigned char) + 2 * sizeof(unsigned int);
    result += sizeof(fixed_decision_diagram_type);
    return result;
}

inline void hashbyte(unsigned int &H, unsigned int x) {
    H = 37 * H ^ x;
}

inline void hashword(unsigned int &H, unsigned int x) {
    hashbyte(H,  x >> 24);
    hashbyte(H, (x & 0x0ff0000) >> 16);
    hashbyte(H, (x & 0x000ff00) >> 8);
    hashbyte(H,  x & 0x00000ff);
}

inline unsigned int hashnode(
    unsigned char level,
    unsigned int child0,
    unsigned int child1
) {
    unsigned int H = 0x480bdd;
    hashbyte(H, level);
    hashword(H, child0);
    hashword(H, child1);
    return H;
}

unsigned int fixed_decision_diagram_type::add_unique(
    unsigned char level,
    unsigned int child0,
    unsigned int child1
) {
    assert(level);
    if (next_node >= max_nodes) {
        enlarge_unique_table(next_node+1);
    }

    // Hash node
    unsigned int H = hashnode(level, child0, child1) % max_nodes;

    // Check all nodes in this bucket for equality
    for (unsigned int curr = unique_table[H]; curr;
     curr = unique_table_next[curr]) {
        if (node_level[curr] != level)   continue;
        if (node_child0[curr] != child0) continue;
        if (node_child1[curr] != child1) continue;
        return curr;
    }

    // This is a new node; create it and add to unique_table
    node_level[next_node] = level;
    node_child0[next_node] = child0;
    node_child1[next_node] = child1;
    unique_table_next[next_node] = unique_table[H];
    unique_table[H] = next_node;
    return next_node++;
}

void fixed_decision_diagram_type::enlarge_unique_table(unsigned int req_max) {
    if (0 == req_max) {
        bdd_terminate("EE: Unique table overflow.");
    }
    if (req_max <= max_nodes) return;

    unsigned int u;
    unsigned int rm{1023};
    while (rm < req_max) {
        rm *= 2;
        ++rm;
    }

    //
    // Enlarge node and unique_table storage
    //
    // TODO: It's unclear why we're using std::realloc() and raw pointers at all,
    // instead of just using a std::vector and its resize() function (which would
    // simplify several things.)
    //

    node_level = static_cast<unsigned char *>(std::realloc(node_level, rm));
    if (nullptr == node_level) {
        bdd_terminate("EE: realloc fail: node level.");
    }
    // TODO: Use std::fill?
    for (u = max_nodes; u < rm; ++u) node_level[u] = 0;

    node_child0 = static_cast<unsigned int *>(std::realloc(
     node_child0, rm * sizeof(unsigned int)));
    if (nullptr == node_child0) {
        bdd_terminate("EE: realloc fail: node_child0.");
    }

    node_child1 = static_cast<unsigned int *>(std::realloc(
     node_child1, rm * sizeof(unsigned int)));
    if (nullptr == node_child1) {
        bdd_terminate("EE: realloc fail: node_child1.");
    }

    unique_table = static_cast<unsigned int *>(std::realloc(
     unique_table, rm * sizeof(unsigned int)));
    if (nullptr == unique_table) {
        bdd_terminate("EE: realloc fail: unique_table.");
    }

    unique_table_next = static_cast<unsigned int *>(std::realloc(
     unique_table_next, rm * sizeof(unsigned int)));
    if (nullptr == unique_table_next) {
        bdd_terminate("EE: realloc fail: unique_table_next.");
    }

    //
    // Re-hash everything into unique_table
    //
    for (u = 0; u < rm; u++) {
        unique_table[u] = 0;
        unique_table_next[u] = 0;
    }
    for (u = 0; u < terminals; u++) {
        node_level[u] = 0;
    }
    for (u = terminals; u < next_node; u++) {
        if (0 == node_level[u]) continue;
        unsigned int H = hashnode(node_level[u], node_child0[u],
         node_child1[u]) % rm;
        unique_table_next[u] = unique_table[H];
        unique_table[H] = u;
    }

    max_nodes = rm;
}
