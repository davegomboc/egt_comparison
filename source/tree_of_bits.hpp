#ifndef EGT_COMPARISON_TREE_OF_BITS_HPP
#define EGT_COMPARISON_TREE_OF_BITS_HPP


#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "bit_stuff.hpp"
#include "uint_type_selection.hpp"


template <
    std::size_t const required_per_leaf_node_bit_count,
    std::size_t const required_per_internal_node_bit_count,
    std::size_t const required_tree_node_index_bit_count,
    std::size_t const branch_count
> class tree_of_bits_tt {
public:
    using node_index_type
     = selected_uint_tt<required_tree_node_index_bit_count>;
    using leaf_type = selected_uint_tt<required_per_leaf_node_bit_count>;
    using internal_type
     = selected_uint_tt<required_per_internal_node_bit_count>;

    static std::size_t constexpr indexable_node_limit{
     static_cast<std::size_t>(1) << required_tree_node_index_bit_count};

    class node_type {
        friend class tree_of_bits_tt;

        static std::size_t constexpr per_node_bit_count{1 + std::max(
         required_per_leaf_node_bit_count, required_per_internal_node_bit_count
         + required_tree_node_index_bit_count * branch_count)};
        using node_t = selected_uint_tt<per_node_bit_count>;

        // Bit 0 is true iff the node is a leaf.
        static node_t constexpr is_node_a_leaf_shift{};
        static node_t constexpr is_node_a_leaf_mask{just_lsbs_set<node_t>(1)};

        // If the node is a leaf, then bits > 0 contain leaf node data.
        static node_t constexpr leaf_node_data_shift{1};
        static node_t constexpr leaf_node_data_mask{
         just_lsbs_set<node_t>(required_per_leaf_node_bit_count)};

        static node_t constexpr internal_node_data_shift{1};
        static node_t constexpr internal_node_data_mask{
         just_lsbs_set<node_t>(required_per_internal_node_bit_count)};

        static node_t constexpr branch_data_shift(
            std::size_t const nth_branch
        ) {
            return internal_node_data_shift
             + required_per_internal_node_bit_count
             + nth_branch * required_tree_node_index_bit_count;
        }
        static node_t constexpr branch_data_mask{
         just_lsbs_set<node_t>(required_tree_node_index_bit_count)};

        node_t data;

    public:
        node_type & operator=(node_type const &) = default;
        node_type & operator=(node_type &&) = default;
        node_type(node_type const &) = default;
        node_type(node_type &&) = default;
        ~node_type() = default;
        node_type() = default;

        // TODO: Eliminate this non-const ref overload.
        node_type(node_type & n) : data(n.data) {}

        // create leaf node
        template <typename tob_leaf_type>
        node_type(tob_leaf_type && val) {
            data = (static_cast<node_t>(val) << leaf_node_data_shift)
             | (is_node_a_leaf_mask << is_node_a_leaf_shift);
        }

        // should be number of ptrs == branch_count
        template <typename tob_internal_type, typename... Is>
        node_type(
            tob_internal_type const & c,
            node_index_type ptr1, Is... ptrs
        ) {
            data = (static_cast<node_t>(c) & internal_node_data_mask)
             << internal_node_data_shift;
            std::size_t i{};
            // This compiles when all Is are node_index_type.
            for (auto const p : {ptr1, ptrs...}) {
                data |= (static_cast<node_t>(p) & branch_data_mask)
                 << branch_data_shift(i++);
            }
        }

        template <typename tob_internal_type>
        node_type(
            tob_internal_type const & c,
            node_index_type const ptrs[branch_count]
        ) {
            data = (static_cast<node_t>(c) & internal_node_data_mask)
             << internal_node_data_shift;
            for (std::size_t i{}; i != branch_count; ++i) {
                data |= (static_cast<node_t>(ptrs[i]) & branch_data_mask)
                 << branch_data_shift(i);
            }
        }

        bool is_leaf() const {
            return (data >> is_node_a_leaf_shift) & is_node_a_leaf_mask;
        }

        leaf_type as_leaf_value() const {
            assert(is_leaf());
            return (data >> leaf_node_data_shift) & leaf_node_data_mask;
        }

        internal_type as_internal_value() const {
            assert(not is_leaf());
            return (data >> internal_node_data_shift) & internal_node_data_mask;
        }

        node_index_type branch(std::size_t const nth_branch) const {
            assert(not is_leaf());
            return (data >> branch_data_shift(nth_branch)) & branch_data_mask;
        }

        std::string to_string() const {
            return to_binary_string(data);
        }

        std::size_t hash() const {
            return bithash<node_t>()(data);
        }

        bool operator==(node_type const & other) const {
            return data == other.data;
        }
    };

    template <typename node_fn_type, typename leaf_fn_type>
    std::string to_string(node_fn_type node_fn, leaf_fn_type leaf_fn) const {
        return strfull(the_root_node_index, node_fn, leaf_fn);
    }

    template <typename... Ts>
    node_index_type add_node(Ts &&... args) {
        if (the_node_reference_counts.size() != the_nodes.size()) {
            rebuild_caches();
        }
        node_type new_node(std::forward<Ts>(args)...);
        auto const node_index_iter{the_node_to_index_map.find(new_node)};
        if (node_index_iter == the_node_to_index_map.end()) {
            // We will assume that every node being added will actually be
            // referred to.
            the_nodes.emplace_back(new_node);
            the_node_reference_counts.emplace_back(1);
            if (the_nodes.size() > indexable_node_limit) {
                std::cerr << "EE: There are too many nodes in the tree for all "
                 "of them to be indexed.  Use a higher value for "
                 "required_tree_node_index_bit_count.\n";
                std::exit(EXIT_FAILURE);
            }
            node_index_type const result{
             static_cast<node_index_type>(the_nodes.size() - 1)};
            the_node_to_index_map[new_node] = result;
            return result;
        } else {
            ++the_node_reference_counts[node_index_iter->second];
            return node_index_iter->second;
        }
    }

    template <typename... Ts>
    node_index_type change_node(node_index_type node_index, Ts &&... args) {
        // N.B. We do not bother to
        //     --the_node_reference_counts[node_index];
        // because we also do not do compaction.
        node_type new_node(std::forward<Ts>(args)...);
        if (the_node_reference_counts[node_index] > 1) {
            return add_node(new_node);
        }
        the_node_to_index_map.erase(the_nodes[node_index]);
        the_nodes[node_index] = new_node;
        the_node_to_index_map[new_node] = node_index;
        return node_index;
    }

    // Returns true iff an error occurred.
    [[nodiscard]]
    bool serialize(std::ostream & data_out) const {
        std::size_t const tsize{the_nodes.size()};
        data_out.write(reinterpret_cast<char const *>(&tsize), sizeof(tsize));
        std::size_t const write_size{tsize * sizeof(node_type)};
        assert(write_size <= std::numeric_limits<std::streamsize>::max());
        data_out.write(reinterpret_cast<char const *>(the_nodes.data()),
         static_cast<std::streamsize>(write_size));
        std::size_t const troot{the_root_node_index};
        data_out.write(reinterpret_cast<char const *>(&troot), sizeof(troot));
        if (data_out.fail()) {
            std::cerr << "EE: ostream failure while serializing tree of bits."
             "\n";
        }
        return data_out.fail();
    }

    tree_of_bits_tt & operator=(tree_of_bits_tt const &) = default;
    tree_of_bits_tt & operator=(tree_of_bits_tt &&) = default;
    tree_of_bits_tt(tree_of_bits_tt const &) = default;
    tree_of_bits_tt(tree_of_bits_tt &&) = default;
    ~tree_of_bits_tt() = default;

    tree_of_bits_tt() {
        the_root_node_index = static_cast<decltype(the_root_node_index)>(-1);
    }

    [[nodiscard]]
    static std::optional<tree_of_bits_tt<
        required_per_leaf_node_bit_count,
        required_per_internal_node_bit_count,
        required_tree_node_index_bit_count,
        branch_count
    >> create_opt(std::istream & data_in) {
        tree_of_bits_tt<
            required_per_leaf_node_bit_count,
            required_per_internal_node_bit_count,
            required_tree_node_index_bit_count,
            branch_count
        > result;
        std::size_t tsize;
        data_in.read(reinterpret_cast<char *>(&tsize), sizeof(tsize));
        result.the_nodes.resize(tsize);
        std::size_t const read_size{tsize * sizeof(node_type)};
        assert(read_size <= std::numeric_limits<std::streamsize>::max());
        data_in.read(reinterpret_cast<char *>(result.the_nodes.data()),
         static_cast<std::streamsize>(read_size));
        std::size_t troot;
        data_in.read(reinterpret_cast<char *>(&troot), sizeof(troot));
        // TODO: Handle invalid data in some reasonable way.
        assert(troot < tsize);
        assert(troot
         <= std::numeric_limits<decltype(result.the_root_node_index)>::max());
        result.the_root_node_index
         = static_cast<decltype(result.the_root_node_index)>(troot);

        if (data_in.fail()) {
            std::cerr << "EE: A failure occurred while deserializing a tree of "
             "bits.\n";
            return std::nullopt;
        }

        // We intentionally defer calling rebuild_caches(): it is not yet
        // necessary to do so.
        return result;
    }

    std::size_t nbytes() const {
        return sizeof(std::size_t) + the_nodes.size() * sizeof(node_type)
         + sizeof(node_index_type);
    }

    // if it were a tree and not a DAG
    std::size_t nleaves() const {
        return nleavesimpl(the_root_node_index);
    }

    // as a DAG
    std::size_t nnodes() const {
        return the_nodes.size();
    }

    // should always be false!
    bool has_cycle() const {
        std::vector<node_index_type> path;
        return check_for_cycle(the_root_node_index, path);
    }

    std::size_t bits_per_node() const {
        return sizeof(node_type) * CHAR_BIT;
    }

    // TODO?  Write generic DFS
    //   problem: how generic?  attempts end up working for some types
    //     of searches but not others

    // instead, I'm giving access to the internals through these
    // accessors... not great.
    void set_root_node_index(node_index_type const new_root_node_index) {
        the_root_node_index = new_root_node_index;
    }

    node_index_type root_node_index() const {
        return the_root_node_index;
    }

    bool is_leaf(node_index_type const node_index) const {
        return the_nodes[node_index].is_leaf();
    }

    auto as_leaf_value(node_index_type const node_index) const {
        return the_nodes[node_index].as_leaf_value();
    }

    auto as_internal_value(node_index_type const node_index) const {
        return the_nodes[node_index].as_internal_value();
    }

    node_index_type branch(
        node_index_type const node_index,
        std::size_t const nth_branch
    ) const {
        return the_nodes[node_index].branch(nth_branch);
    }

private:
    struct node_hasher_t {
        std::size_t operator()(node_type const & node) const {
            return node.hash();
        }
    };

    std::vector<node_type> the_nodes;
    node_index_type the_root_node_index;
    // The fields below cache derived information, and so do not need to be
    // serialized.
    std::unordered_map<node_type, node_index_type, node_hasher_t>
     the_node_to_index_map;
    std::vector<std::size_t> the_node_reference_counts;

    void rebuild_caches() {
        the_node_reference_counts.resize(the_nodes.size());
        std::fill(the_node_reference_counts.begin(),
         the_node_reference_counts.end(), 0);
        ++the_node_reference_counts[the_root_node_index];
        for (node_index_type node_index{}; node_index != the_nodes.size();
         ++node_index) {
            the_node_to_index_map[the_nodes[node_index]] = node_index;
            for (std::size_t nth_branch{}; nth_branch != branch_count;
             ++nth_branch) {
                ++the_node_reference_counts[
                 the_nodes[node_index].branch(nth_branch)];
            }
        }
    }

    // could be faster!!
    bool check_for_cycle(
        node_index_type const node_index,
        std::vector<node_index_type> & path
    ) const {
        auto node_index_iter{
         std::find(path.begin(), path.end(), node_index)};
        if (node_index_iter != path.end()) {
            for (auto const & x : path) {
                std::cerr << x << ' ';
            }
            std::cerr << node_index << '\n';
            for (; node_index_iter != path.end(); ++node_index_iter) {
                std::cerr << *node_index_iter << ": "
                 << the_nodes[*node_index_iter].to_string() << "  "
                 << the_nodes[*node_index_iter].is_leaf() << '\n';
            }
            return true;
        }
        if (not the_nodes[node_index].is_leaf()) {
            path.emplace_back(node_index);
            for (std::size_t i{}; i != branch_count; ++i) {
                if (check_for_cycle(the_nodes[node_index].branch(i), path)) {
                    return true;
                }
            }
            path.pop_back();
        }
        return false;
    }

    // could be done faster
    std::size_t nleavesimpl(node_index_type const node_index) const {
        if (the_nodes[node_index].is_leaf()) {
            return 1;
        }
        std::size_t result{};
        for (std::size_t nth_branch{}; nth_branch != branch_count;
         ++nth_branch) {
            result += nleavesimpl(the_nodes[node_index].branch(nth_branch));
        }
        return result;
    }

    template <typename node_fn_type, typename leaf_fn_type>
    std::string strfull(
        node_index_type const node_index,
        node_fn_type const node_fn,
        leaf_fn_type const leaf_fn,
        std::string const above_prefix = "",
        std::string const at_prefix = "",
        std::string const below_prefix = ""
    ) const {
        /*
        char constexpr blank[] = "  ";
        char constexpr vert[] = "| ";
        char constexpr bottom_corner[] = "\\-";
        char constexpr top_corner[] = "/-";
        char constexpr tee_left[] = "-+";
        char constexpr tee_right[] = "+-";
        char constexpr four_way[] = "-+-";
        */
        char constexpr blank[] = "  ";
        char constexpr vertical[] = "│ ";
        char constexpr bottom_corner[] = "└─";
        char constexpr top_corner[] = "┌─";
        char constexpr tee_left[] = "─┤";
        char constexpr tee_right[] = "├─";
        char constexpr four_way[] = "─┼─";
        if (the_nodes[node_index].is_leaf()) {
            return at_prefix + leaf_fn(the_nodes[node_index].as_leaf_value())
             + "\n";
        }
        std::string const nstr(
         node_fn(the_nodes[node_index].as_internal_value()));
        std::string const pad(nstr.size() + 1, ' ');
        std::string result{};
        for (std::size_t nth_branch{}; nth_branch < branch_count / 2;
         ++nth_branch) {
            result += strfull(
             the_nodes[node_index].branch(nth_branch),
             node_fn,
             leaf_fn,
             above_prefix + pad + (nth_branch == 0 ? blank : vertical),
             above_prefix + pad + (nth_branch == 0 ? top_corner : tee_right),
             above_prefix + pad + vertical);
        }
        if (branch_count % 2 == 1) {
            result += strfull(
             the_nodes[node_index].branch(branch_count / 2),
             node_fn,
             leaf_fn,
             above_prefix + pad + (branch_count > 1 ? vertical : blank),
             at_prefix + nstr + four_way,
             below_prefix + pad + (branch_count > 1 ? vertical : blank));
        } else {
            result += at_prefix + nstr + tee_left + "\n";
        }
        for (std::size_t nth_branch = (branch_count + 1) / 2;
         nth_branch < branch_count; ++nth_branch) {
            result += strfull(
             the_nodes[node_index].branch(nth_branch),
             node_fn,
             leaf_fn,
             below_prefix + pad + vertical,
             below_prefix + pad + (nth_branch == branch_count - 1
              ? bottom_corner : tee_right),
             below_prefix + pad + (nth_branch == branch_count - 1
              ? blank : vertical));
        }
        return result;
    }
};


#endif // EGT_COMPARISON_TREE_OF_BITS_HPP
