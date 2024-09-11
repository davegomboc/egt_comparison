#ifndef EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_HPP


#include <cassert>
#include <iostream>
#include <vector>


/**
  Class for reading and querying BDDs that won't change.

  TODO: Split off the unique table functionality into a separate class and refer
  to it via std::weak_ptr, because the unique table data portion of this class
  doesn't necessarily have the same lifetime as the remainder of the data
  managed by this class.
*/
class fixed_decision_diagram_type {
  public:
    fixed_decision_diagram_type & operator=(
        fixed_decision_diagram_type const &) = delete;
    fixed_decision_diagram_type & operator=(
        fixed_decision_diagram_type && other);
    fixed_decision_diagram_type(fixed_decision_diagram_type const &) = delete;
    fixed_decision_diagram_type(fixed_decision_diagram_type && other);
    ~fixed_decision_diagram_type();
    fixed_decision_diagram_type();

    /**
      Read a BDD from a file.
        @param  pathname    File to read from.  Input files may be
                            compressed with the following:
                              .gz, .bz2, .xz

        @return   A handle to the root node in the file.

        TODO: Provide a version that uses std::istream directly, so that test
        code can be hermetic.

        TODO: Use std::filesystem::path instead of char const *.

        TODO: Also return the file node count from this function, instead of
        storing it as a side effect of calling.  (std::pair/tuple return type.)
    */
    unsigned int add_nodes_from_file(char const * pathname);

    /**
      Indicates that we are done building, and
      all remaining operations will be read-only queries.
      This allows us to free some data structures
      used during building (like the unique table).

      TODO: Split unique table into a separate class (see class header notes.)
    */
    void done_building();

    template <typename uint_type>
    unsigned int evaluate(
        unsigned int bdd_node_id,
        uint_type const * const zeroth_element_of_contiguous_memory_ptr
    ) const {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
/*
        std::cout << "DD: Entered evaluate(" << bdd_node_id << ", 0x"
         << std::hex << reinterpret_cast<std::uintptr_t>(
         zeroth_element_of_contiguous_memory_ptr) << ")." << std::dec
         << std::endl;
*/
        assert(nullptr != zeroth_element_of_contiguous_memory_ptr);
        while (0 != node_level[bdd_node_id]) {
            assert(bdd_node_id < next_node);
            bool const bit_is_set{0 != *(zeroth_element_of_contiguous_memory_ptr
             + node_level[bdd_node_id])};
            //std::cout << "TT: bit_is_set = " << bit_is_set << std::endl;
            bdd_node_id = bit_is_set ? node_child1[bdd_node_id]
             : node_child0[bdd_node_id];
            //std::cout << "TT: bdd_node_id = " << bdd_node_id << std::endl;
        }
/*
        std::cout << "DD: Exiting evaluate, returning " << bdd_node_id << "."
         << std::endl;
*/
        return bdd_node_id;
    }

    // TODO: Use a zero-based std::vector instead of a one-based std::vector, to
    // avoid the need to insert a junk element (which takes O(n)) before
    // probing.  Using a pointer offset by one (but never dereferencing it
    // before adding the one back) would allow std::vector to be used normally
    // while still allowing hardcoded arrays using an unused zero spot.
    template <typename uint_type>
    unsigned int evaluate(
        unsigned int bdd_node_id,
        std::vector<uint_type> const & vars
    ) const {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
/*
        std::cout << "DD: Entered evaluate(" << bdd_node_id << ", "
         "std::vector<uint_type> const &)." << std::endl;
        std::cout << "TT: vars.size() = " << vars.size() << std::endl;
        for (std::size_t i{}; i != vars.size(); ++i) {
            std::cout << "TT: vars[" << i << "] = "
             << static_cast<bool>(vars[i]);
            if (0 == i) {
                std::cout << " (which will be ignored!)";
            } else if (1 == i) {
                std::cout << " (side to move)";
            }
            std::cout << '\n';
        }
*/
        if constexpr (std::is_same_v<bool, uint_type>) {
            std::vector<unsigned int> uint_vars(vars.begin(), vars.end());
            bdd_node_id = this->evaluate<unsigned int>(bdd_node_id,
             uint_vars.data());
        } else {
            bdd_node_id = this->evaluate<uint_type>(bdd_node_id, vars.data());
        }
/*
        std::cout << "DD: Exiting evaluate, returning " << bdd_node_id
         << "." << std::endl;
*/
        return bdd_node_id;
    }

    /**
      Evaluate a function for given variable assignments.
        @param  f_root  Handle to the root node of the function
        @param  vars    variable assignments, where vars[i]
                        indicates the assignment to variable i.
                        vars should be dimension 1+levels or higher.

      TODO: Note that this signature is not easily usable with std::vector<bool>
      because that type usually uses a multiple-bit-per-address representation.
      TODO: Retire this function in favour of the templated versions above.
    */
    unsigned int evaluate(unsigned int f_root, bool const * vars) const;

    unsigned long in_use_byte_count() const;
    unsigned int level_count() const { return levels; }
    unsigned int terminal_node_count() const { return terminals; }
    unsigned int nonterminal_node_count() const {
        return next_node - terminals;
    }
    unsigned int file_node_count() const { return file_nodes; }

  private:
    unsigned int add_unique(
        unsigned char level,
        unsigned int child0,
        unsigned int child1);
    void enlarge_unique_table(unsigned int req_max);

    unsigned char levels;
    unsigned int terminals;
    unsigned int next_node;
    unsigned int max_nodes;

    // nodes in last file read
    //
    // TODO: It's not clear why this is being stored instead of being returned
    // from the function that generates it, e.g.,
    //     auto [bdd_node_id, file_node_count] = add_nodes_from_file(...);
    unsigned int file_nodes;

    // Node storage
    unsigned char * node_level;
    unsigned int * node_child0;
    unsigned int * node_child1;

    // Unique table
    unsigned int * unique_table;
    unsigned int * unique_table_next;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_FIXED_DECISION_DIAGRAM_TYPE_HPP
