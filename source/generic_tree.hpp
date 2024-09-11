#ifndef EGT_COMPARISON_GENERIC_TREE_HPP
#define EGT_COMPARISON_GENERIC_TREE_HPP


#include <climits>
#include <cstddef>
#include <string>
#include <type_traits>

#include "tree_of_bits.hpp"


template <
    typename leaf_type,
    typename internal_type,
    std::size_t const required_tree_node_index_bit_count,
    std::size_t const branch_count,
    typename = std::enable_if_t<std::is_trivially_copyable_v<leaf_type>
     and std::is_trivially_copyable_v<internal_type>, void>
> class generic_tree_tt {
private:
    using tree_of_bits_type = tree_of_bits_tt<
        sizeof(leaf_type) * CHAR_BIT,
        sizeof(internal_type) * CHAR_BIT,
        required_tree_node_index_bit_count,
        branch_count
    >;
    using tob_leaf_type = typename tree_of_bits_type::leaf_type;
    using tob_internal_type = typename tree_of_bits_type::internal_type;

    union leaf_union {
        leaf_type as_leaf;
        tob_leaf_type as_tob_leaf;
        leaf_union(leaf_type leaf, bool) : as_leaf(std::move(leaf)) {}
        leaf_union(tob_leaf_type tob_leaf) : as_tob_leaf(std::move(tob_leaf)) {}
    };

    union internal_union {
        internal_type as_internal;
        tob_internal_type as_tob_internal;
        internal_union(internal_type internal, bool)
          : as_internal(std::move(internal)) {}
        internal_union(tob_internal_type tob_internal)
          : as_tob_internal(std::move(tob_internal)) {}
    };

    static leaf_type from_tob_leaf(tob_leaf_type tob_leaf) {
        return leaf_union(std::move(tob_leaf)).as_leaf;
    }
    static tob_leaf_type to_tob_leaf(leaf_type leaf) {
        return leaf_union(std::move(leaf), true).as_tob_leaf;
    }
    static internal_type from_tob_internal(tob_internal_type tob_internal) {
        return internal_union(std::move(tob_internal)).as_internal;
    }
    static tob_internal_type to_tob_internal(internal_type internal) {
        return internal_union(std::move(internal), true).as_tob_internal;
    }

    tree_of_bits_type the_tree_of_bits;

public:
    using node_index_type = typename tree_of_bits_type::node_index_type;

    generic_tree_tt & operator=(generic_tree_tt const &) = default;
    generic_tree_tt & operator=(generic_tree_tt &&) = default;
    generic_tree_tt(generic_tree_tt const &) = default;
    generic_tree_tt(generic_tree_tt &&) = default;
    ~generic_tree_tt() = default;
    generic_tree_tt() = default;

    [[nodiscard]]
    static std::optional<generic_tree_tt<
        leaf_type,
        internal_type,
        required_tree_node_index_bit_count,
        branch_count
    >> create_opt(std::istream & data_in) {
        std::optional<tree_of_bits_type> tree_of_bits_opt{
         tree_of_bits_type::create_opt(data_in)};
        if (not tree_of_bits_opt) {
            return std::nullopt;
        }
        generic_tree_tt<leaf_type, internal_type,
         required_tree_node_index_bit_count, branch_count> result;
        result.the_tree_of_bits = std::move(*tree_of_bits_opt);
        return result;
    }

    std::size_t bits_per_node() const {
        return the_tree_of_bits.bits_per_node();
    }

    node_index_type branch(
        node_index_type const node_index,
        std::size_t const nth_branch
    ) const {
        return the_tree_of_bits.branch(node_index, nth_branch);
    }

    std::size_t nbytes() const {
        return the_tree_of_bits.nbytes();
    }

    std::size_t nleaves() const {
        return the_tree_of_bits.nleaves();
    }

    std::size_t nnodes() const {
        return the_tree_of_bits.nnodes();
    }

    node_index_type root_node_index() const {
        return the_tree_of_bits.root_node_index();
    }

    void set_root_node_index(node_index_type const new_root_node_index) {
        the_tree_of_bits.set_root_node_index(new_root_node_index);
    }

    // Returns true iff an error occurred.
    [[nodiscard]]
    bool serialize(std::ostream & data_out) const {
        bool const result{the_tree_of_bits.serialize(data_out)};
        if (result) {
            std::cerr << "EE: ostream failing while serializing "
             "generic_tree.\n";
        }
        return result;
    }

    bool is_leaf(node_index_type const node_index) const {
        return the_tree_of_bits.is_leaf(node_index);
    }

    leaf_type as_leaf_value(node_index_type const node_index) const {
        return from_tob_leaf(the_tree_of_bits.as_leaf_value(node_index));
    }
    internal_type as_internal_value(node_index_type const node_index) const {
        return from_tob_internal(the_tree_of_bits.as_internal_value(
         node_index));
    }

    node_index_type add_leaf(leaf_type leaf) {
        return the_tree_of_bits.add_node(to_tob_leaf(std::move(leaf)));
    }
    node_index_type add_internal(
        internal_type internal,
        node_index_type const (&subtrees)[branch_count]
    ) {
        return the_tree_of_bits.add_node(to_tob_internal(std::move(internal)),
         subtrees);
    }

    node_index_type change_to_leaf(
        node_index_type const node_index,
        leaf_type leaf
    ) {
        return the_tree_of_bits.change_node(node_index,
         to_tob_leaf(std::move(leaf)));
    }

    node_index_type change_to_internal(
        node_index_type const node_index,
        internal_type internal,
        node_index_type const (&subtrees)[branch_count]
    ) {
        return the_tree_of_bits.change_node(node_index,
         to_tob_internal(std::move(internal)), subtrees);
    }

    template <typename node_fn_type, typename leaf_fn_type>
    std::string to_string(
        node_fn_type const node_fn,
        leaf_fn_type const leaf_fn
    ) const {
        return the_tree_of_bits.to_string(
         [&node_fn](tob_internal_type const & tob_internal) {
             return node_fn(from_tob_internal(tob_internal));
         },
         [&leaf_fn](tob_leaf_type const & tob_leaf) {
             return leaf_fn(from_tob_leaf(tob_leaf));
         });
    }
};


#endif // EGT_COMPARISON_GENERIC_TREE_HPP
