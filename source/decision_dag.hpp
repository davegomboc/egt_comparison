#ifndef EGT_COMPARISON_DECISION_DAG_HPP
#define EGT_COMPARISON_DECISION_DAG_HPP


#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cube_binary_inputs.hpp"
#include "cube_list.hpp"
#include "hash_combine.hpp"
#include "integer_logarithms.hpp"
#include "parallel_for_each.hpp"
#include "uint_type_selection.hpp"


// TODO: Use generic tree to implement decision DAG.

static std::size_t nleft{};
static std::size_t nsaved{};

template <
    std::size_t const NDIM,
    std::size_t const MAXOUTBITS,
    std::size_t const INDEXBITS
> class ddag {

public:
    // type for indexing bits/dimensions in input
    static std::size_t constexpr DIMBITS{ceil_log2(NDIM)};
    using dimtype = selected_uint_tt<DIMBITS>;
    // type for indexing vector of nodes
    using itype = selected_uint_tt<INDEXBITS>;
    using itype1 = selected_uint_tt<INDEXBITS + 1>;
    static itype constexpr imask{just_lsbs_set<itype>(INDEXBITS)};

    // type for isleaf + onbranch/output
    using ontype = selected_uint_tt<std::max(INDEXBITS, MAXOUTBITS) + 1>;
    // type for dim + offbranch
    using offtype = selected_uint_tt<INDEXBITS + DIMBITS>;

    using outtype = selected_uint_tt<MAXOUTBITS>;

    static offtype constexpr dimmask{just_lsbs_set<offtype>(DIMBITS)};
    static ontype constexpr outmask{just_lsbs_set<ontype>(MAXOUTBITS)};

    static std::size_t constexpr maxsize{
     static_cast<std::size_t>(1) << INDEXBITS};

    class node {
        ontype on;
        offtype off;

    public:
        node(node const & n) : on(n.on), off(n.off) {}
        // TODO: Eliminate the non-const ref overload.
        node(node & n) : on(n.on), off(n.off) {}
        node(node && n) : on(std::move(n.on)), off(std::move(n.off)) {}

        template <typename OUTT>
        node(OUTT && leafval) {
            on = (static_cast<ontype>(leafval) << 1) | 1;
            off = 0;
        }
        node(dimtype const i, itype const offptr, itype const onptr) {
            on = static_cast<ontype>(onptr) << 1;
            off = static_cast<offtype>(offptr) << DIMBITS
             | static_cast<offtype>(i);
        }

        node() {}

        bool isleaf() const {
            return on & 1;
        }

        template <typename OTYPE = std::uint8_t>
        OTYPE leafval() const {
            return (on >> 1) & outmask;
        }

        dimtype dim() const {
            return off & dimmask;
        }
        itype offbranch() const {
            return off >> DIMBITS;
        }
        itype onbranch() const {
            return on >> 1;
        }

        std::string to_string() const {
            return to_binary_string(on) + '|' + to_binary_string(off) + '|'
             + to_binary_string(dimmask) + " " + std::to_string(DIMBITS) + " "
             + to_binary_string(static_cast<offtype>(1) << (DIMBITS + 1)) + " "
             + to_binary_string((static_cast<offtype>(1) << (DIMBITS + 1)) - 1);
        }

        bool operator==(node const & n) const {
            return on == n.on && off == n.off;
        }

        std::size_t hash() const {
            std::size_t ret{};
            hash_combine(ret, on);
            hash_combine(ret, off);
            return ret;
        }
    };

    template <typename CTYPE>
    outtype lookup(CTYPE const & x) const {
        // TODO: Handle the case where the tree is empty.
        itype n = root;
        while (!tree[n].isleaf()) {
            switch (x[tree[n].dim()]) {
            case lde_on:
                n = tree[n].onbranch();
                break;
            case lde_off:
                n = tree[n].offbranch();
                break;
            // TODO: other cases pose problems at this point
            case lde_sng:
            case lde_dc:
            default:
                break;
            }
        }
        return tree[n].leafval();
    }

    template <typename CTYPE>
    std::set<outtype>
    lookupall(CTYPE const & x, std::size_t const result_size_limit = 0) const {
        return lookupallimpl(root, x, result_size_limit);
    }


    template <std::size_t CMAXINBITS, std::size_t CMAXOUTBITS>
    explicit ddag(cube_list_tt<CMAXINBITS, CMAXOUTBITS> const & l) {
        assert(l.ninbits() <= std::numeric_limits<decltype(ndim)>::max());
        ndim = static_cast<decltype(ndim)>(l.ninbits());
        assert(l.noutbits() <= std::numeric_limits<decltype(noutbits)>::max());
        noutbits = static_cast<decltype(noutbits)>(l.noutbits());
        if (ndim > NDIM || noutbits > MAXOUTBITS) {
            std::cerr << "EE: tree not large enough for cube list: " << ndim
             << " versus " << NDIM << " and " << noutbits << " versus "
             << MAXOUTBITS << '\n';
            std::exit(EXIT_FAILURE);
        }
        std::vector<std::size_t> active(l.size());
        std::iota(active.begin(), active.end(), 0);
        std::vector<dimtype> dleft(l.ninbits());
        std::iota(dleft.begin(), dleft.end(), 0);

        nleft = active.size();
        root = build(l, active, 0, active.size(), dleft, dleft.size());
        buildtable.clear();
    }

    std::string to_string() const {
        return strfull(root);
    }

    // TODO: Detect and report when serialization fails, as has already been
    // done with cube lists.
    void serialize(std::ostream & os) const {
        std::size_t const tsize = tree.size();
        std::size_t const troot = root;
        std::size_t const tndim = ndim;
        std::size_t const tnoutbits = noutbits;
        std::size_t const andim = NDIM;
        std::size_t const aoutbits = MAXOUTBITS;
        os.write(reinterpret_cast<char const *>(&tsize), sizeof(tsize));
        os.write(reinterpret_cast<char const *>(&tndim), sizeof(tndim));
        os.write(reinterpret_cast<char const *>(&tnoutbits), sizeof(tnoutbits));
        os.write(reinterpret_cast<char const *>(&andim), sizeof(andim));
        os.write(reinterpret_cast<char const *>(&aoutbits), sizeof(aoutbits));
        std::size_t const write_size{tsize * sizeof(node)};
        assert(write_size <= std::numeric_limits<std::streamsize>::max());
        os.write(
         reinterpret_cast<char const *>(&tree[0]),
         static_cast<std::streamsize>(write_size));
        os.write(reinterpret_cast<char const *>(&troot), sizeof(troot));
    }

    // TODO: Create ddag::create_opt and use it instead, as has already been
    // done with cube lists.
    explicit ddag(std::istream & is) {
        std::size_t tsize, tndim, tnoutbits, andim, aoutbits;
        is.read(reinterpret_cast<char *>(&tsize), sizeof(tsize));
        is.read(reinterpret_cast<char *>(&tndim), sizeof(tndim));
        is.read(reinterpret_cast<char *>(&tnoutbits), sizeof(tnoutbits));
        is.read(reinterpret_cast<char *>(&andim), sizeof(andim));
        is.read(reinterpret_cast<char *>(&aoutbits), sizeof(aoutbits));
        assert(tndim <= std::numeric_limits<decltype(ndim)>::max());
        ndim = static_cast<decltype(ndim)>(tndim);
        assert(noutbits <= std::numeric_limits<decltype(noutbits)>::max());
        noutbits = static_cast<decltype(noutbits)>(tnoutbits);
        if (NDIM != andim || MAXOUTBITS != aoutbits) {
            std::cerr << "EE: tree template params don't match: " << andim
             << " versus " << NDIM << " and " << aoutbits << " versus "
             << MAXOUTBITS << '\n';
            std::exit(EXIT_FAILURE);
        }
        if (ndim > NDIM || noutbits > MAXOUTBITS) {
            std::cerr << "EE: tree not large enough for datafile: " << ndim
             << " versus " << NDIM << " and " << noutbits << " versus "
             << MAXOUTBITS << '\n';
            std::exit(EXIT_FAILURE);
        }
        tree.resize(tsize);
        std::size_t const read_size{tsize * sizeof(node)};
        assert(read_size <= std::numeric_limits<std::streamsize>::max());
        is.read(
         reinterpret_cast<char *>(&tree[0]),
         static_cast<std::streamsize>(read_size));
        std::size_t troot;
        is.read(reinterpret_cast<char *>(&troot), sizeof(troot));
        assert(troot <= std::numeric_limits<decltype(root)>::max());
        root = static_cast<decltype(root)>(troot);
    }

    std::size_t nnodes() const {
        return tree.size();
    }
    std::size_t nbytes() const {
        return sizeof(std::size_t) + tree.size() * sizeof(node) + sizeof(itype)
         + sizeof(dimtype) + sizeof(outtype);
    }
    std::size_t ncubes() const {
        // same as number of leaves as a tree
        return nleaves(root);
    }

    cube_list_tt<NDIM, MAXOUTBITS> to_cube_list() const {
        cube_list_tt<NDIM, MAXOUTBITS> result(ndim, noutbits);
        cube_binary_inputs_tt<NDIM> c(lde_dc, ndim);
        buildlst(result, c, root);
        result.ensure_sorted();
        return result;
    }

    bool hascycle() const { // should always be false!
        std::vector<itype> path;
        return checkcycle(root, path);
    }

    dimtype getndim() const {
        return ndim;
    }
    outtype getnoutbits() const {
        return noutbits;
    }

private:
    struct nodehasher {
        std::size_t operator()(const node & n) const {
            return n.hash();
        }
    };

    std::vector<node> tree;
    itype root;
    dimtype ndim;
    outtype noutbits;
    std::unordered_map<node, itype, nodehasher> buildtable;

    // TODO: could be faster!!
    bool checkcycle(itype const n, std::vector<itype> & path) const {
        auto loc = std::find(path.begin(), path.end(), n);
        if (loc != path.end()) {
            for (auto & x : path) { std::cerr << x << ' '; }
            std::cerr << n << '\n';
            for (; loc != path.end(); ++loc) {
                std::cerr << *loc << ": " << tree[*loc].to_string() << "  "
                 << tree[*loc].isleaf() << '\n';
            }
            return true;
        }
        if (not tree[n].isleaf()) {
            path.emplace_back(n);
            if (checkcycle(tree[n].onbranch(), path)) {
                return true;
            }
            if (checkcycle(tree[n].offbranch(), path)) {
                return true;
            }
            path.pop_back();
        }
        return false;
    }

    std::size_t nleaves(itype const n) const {
        if (tree[n].isleaf()) {
            return 1;
        }
        return nleaves(tree[n].onbranch()) + nleaves(tree[n].offbranch());
    }

    template <typename CTYPE>
    std::set<outtype> lookupallimpl(
        itype const n,
        CTYPE const & x,
        std::size_t const result_size_limit = 0
    ) const {
        if (tree[n].isleaf()) {
            std::set<outtype> ret;
            ret.insert(tree[n].leafval());
            return ret;
        } else {
            switch (x[tree[n].dim()]) {
            case lde_on:
                return lookupallimpl(tree[n].onbranch(), x, result_size_limit);
            case lde_off:
                return lookupallimpl(tree[n].offbranch(), x, result_size_limit);
            case lde_dc:
                {
                    auto ret = lookupallimpl(tree[n].onbranch(), x,
                     result_size_limit);
                    if ((0 != result_size_limit)
                     and (result_size_limit <= ret.size())) {
                        return ret;
                    }
                    auto ret2 = lookupallimpl(tree[n].offbranch(), x,
                     result_size_limit);
                    ret.insert(ret2.begin(), ret2.end());
                    return ret;
                }
            case lde_sng:
            default:
                return {}; //???
            }
        }
    }

    void buildlst(
        cube_list_tt<NDIM, MAXOUTBITS> & l,
        cube_binary_inputs_tt<NDIM> & c,
        itype const n
    ) const {
        if (tree[n].isleaf()) {
            l.the_cubes.emplace_back(c, tree[n].leafval());
        } else {
            c.set(tree[n].dim(), lde_off);
            buildlst(l, c, tree[n].offbranch());
            c.set(tree[n].dim(), lde_on);
            buildlst(l, c, tree[n].onbranch());
            c.set(tree[n].dim(), lde_dc);
        }
    }

    std::string strfull(
     itype const n,
     std::string const aboveprefix = "",
     std::string const atprefix = "",
     std::string const belowprefix = "") const {
        if (tree[n].isleaf()) {
            return atprefix + to_binary_string(tree[n].leafval(), noutbits)
             + "\n";
        }
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0')
         << static_cast<std::size_t>(tree[n].dim());
        return strfull(
                tree[n].offbranch(),
                aboveprefix + "      ",
                aboveprefix + "    /-",
                aboveprefix + "    | ")
         + atprefix + ss.str() + "<\n"
         + strfull(
                tree[n].onbranch(),
                belowprefix + "    | ",
                belowprefix + "    \\-",
                belowprefix + "      ");
    }

    template <typename... Ts>
    itype addnode(Ts &&... args) {
        if (buildtable.size() == 0) {
            assert(tree.size() <= std::numeric_limits<unsigned int>::max());
            for (unsigned int i{}; i != tree.size(); ++i) {
                buildtable[tree[i]] = i;
            }
        }
        node nnode(std::forward<Ts>(args)...);
        auto loc = buildtable.find(nnode);
        if (loc == buildtable.end()) {
            tree.emplace_back(nnode);
            if (tree.size() > maxsize) {
                std::cerr << "EE: EXCEED MAXSIZE OF TREE: increase INDEXBITS\n";
                std::exit(EXIT_FAILURE);
            }
            assert(tree.size() <= std::numeric_limits<itype>::max());
            itype const result{static_cast<itype>(tree.size() - 1)};
            buildtable[nnode] = result;
            return result;
        } else {
            ++nsaved;
            return loc->second;
        }
    }

    template <std::size_t CMAXINBITS, std::size_t CMAXOUTBITS, typename DITYPE>
    itype build(
     cube_list_tt<CMAXINBITS, CMAXOUTBITS> const & l,
     std::vector<DITYPE> & active,
     std::size_t a0,
     std::size_t an,
     std::vector<dimtype> & dleft,
     std::size_t dn,
     std::size_t lvl = 0) {
        if (ispure(l, active, a0, an)) {
            std::size_t nnleft = nleft - (an - a0);
            constexpr std::size_t leftmod = 100000;
            if (nnleft / leftmod != nleft / leftmod) {
            }
            nleft = nnleft;
            return addnode(l[active[a0]].outputs());
        }
        auto d = bestdim(l, active, a0, an, dleft, dn, lvl);
        --dn;
        auto a1 = separate(l, active, a0, an, d);
        auto offi = build(l, active, a0, a1, dleft, dn, lvl + 1);
        auto oni = build(l, active, a1, an, dleft, dn, lvl + 1);

        // The following condition can occur if a number of variables produce no
        // gains (but, like in xor, some need to be chosen)
        if (offi == oni) {
            return oni;
        }
        return addnode(d, offi, oni);
    }

    template <std::size_t CMAXINBITS, std::size_t CMAXOUTBITS, typename DITYPE>
    static bool ispure(
     cube_list_tt<CMAXINBITS, CMAXOUTBITS> const & l,
     std::vector<DITYPE> & active,
     std::size_t const a0,
     std::size_t const an) {
        for (std::size_t i = a0 + 1; i < an; i++) {
            if (l[active[i - 1]].outputs() != l[active[i]].outputs()) {
                return false;
            }
        }
        return true;
    }

    template <std::size_t CMAXINBITS, std::size_t CMAXOUTBITS, typename DITYPE>
    static itype separate(
     cube_list_tt<CMAXINBITS, CMAXOUTBITS> const & l,
     std::vector<DITYPE> & active,
     std::size_t a0,
     std::size_t an,
     std::size_t const d) {
        using inputs_type = typename cube_list_tt<CMAXINBITS, CMAXOUTBITS>
         ::cube_type::inputs_type;
        using inputs_index_type = typename inputs_type::index_type;
        // TODO: could be more efficient by walking from both ends and only
        // swapping when necessary
        while (a0 < an) {
            inputs_type const & temp = l[active[a0]].inputs();
            assert(d <= std::numeric_limits<inputs_index_type>::max());
            switch (temp[static_cast<inputs_index_type>(d)]) {
            case lde_off:
                ++a0;
                break;
            case lde_on:
                std::swap(active[a0], active[--an]);
                break;
            // TODO: other cases not clear at the moment
            case lde_dc:
            case lde_sng:
            default:
                break;
            }
        }
        assert(an <= std::numeric_limits<itype>::max());
        return static_cast<itype>(an);
    }

    template <std::size_t CMAXINBITS, std::size_t CMAXOUTBITS, typename DITYPE>
    static dimtype bestdim(
     cube_list_tt<CMAXINBITS, CMAXOUTBITS> const & l,
     std::vector<DITYPE> & active,
     std::size_t const a0,
     std::size_t const an,
     std::vector<dimtype> & dleft,
     std::size_t const dn,
     [[maybe_unused]] std::size_t const lvl = 0) {
        std::vector<std::size_t> indexes(dn);
        std::iota(indexes.begin(), indexes.end(), 0);
        std::vector<double> scores(dn, 0.0);
        desire_parallel_for_each(std::begin(indexes), std::end(indexes),
         [&](auto i) {
            scores[i] = scoredim(l, active, a0, an, dleft[i]);
         });
        auto besti = std::min_element(std::begin(scores), std::end(scores))
         - std::begin(scores);
        assert(0 <= besti);
        std::swap(dleft[dn - 1], dleft[static_cast<std::size_t>(besti)]);
        return dleft[dn - 1];
    }

    template <std::size_t CMAXINBITS, std::size_t CMAXOUTBITS, typename DITYPE>
    static auto scoredim(
     cube_list_tt<CMAXINBITS, CMAXOUTBITS> const & l,
     std::vector<DITYPE> & active,
     std::size_t a0,
     std::size_t an,
     dimtype d) {
        std::map<
         typename cube_list_tt<CMAXINBITS, CMAXOUTBITS>::outputs_value_type,
         std::size_t>
         onhist, offhist;
        std::size_t non = 0, noff = 0;
        for (std::size_t i = a0; i < an; i++) {
            switch (l[active[i]].inputs()[d]) {
            case lde_on:
                ++(onhist[l[active[i]].outputs()]);
                ++non;
                break;
            case lde_off:
                ++(offhist[l[active[i]].outputs()]);
                ++noff;
                break;
            // TODO: other cases pose problems at this point
            case lde_dc:
            case lde_sng:
            default:
                break;
            }
        }
        return entropy(onhist, non) + entropy(offhist, noff);
    }

    template <typename T>
    static double
    entropy(std::map<T, std::size_t> const & h, std::size_t const n) {
        double ret{};
        double dn{static_cast<double>(n)};
        for (auto const & [e, c] : h) {
            if (c > 0) {
                double const c_as_double{static_cast<double>(c)};
                ret -= c_as_double * std::log(c_as_double / dn);
            }
        }
        return ret;
    }
};


#endif // EGT_COMPARISON_DECISION_DAG_HPP
