#include "representation/figurine_placements_type.hpp"


#include <algorithm>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <sstream>

#include "representation/bitboards_type.hpp"
#include "representation/figurine_type.hpp"
#include "representation/figurine_placement_type.hpp"


namespace representation {


// Note that unlike the other constructors for this class, this constructor
// intentionally does not sort the placements.  It has been used for iterating
// over all and perumtations and for sampling from all permutations.
figurine_placements_type::figurine_placements_type(
    material_signature_type const & material_signature,
    coordinates_type const & coordinates
) : the_placements() {
    assert(material_signature.size() == coordinates.size());
    for (std::size_t nth_placement{};
     nth_placement != material_signature.size(); ++nth_placement) {
        the_placements.emplace_back(figurine_placement_type(
         material_signature[nth_placement], coordinates[nth_placement]));
    }
}


figurine_placements_type::figurine_placements_type(
    epd::board_contents_type const & board_contents
) : the_placements{} {
    coordinate_type coordinate{a8};
    while (true) {
        char const ch{
         board_contents[static_cast<unsigned char>(coordinate.as_enum())]};
        if ('1' != ch) {
            std::optional<figurine_type> const figurine_opt{
             figurine_type::create_opt(ch)};
            if (figurine_opt) {
                the_placements.emplace_back(*figurine_opt, coordinate);
            }
        }
        ++coordinate;
        if (a8 == coordinate) {
            break;
        }
    }
    restore_ordering_invariant();
}


figurine_placements_type::figurine_placements_type(
    std::vector<figurine_placement_type> && placements
) : the_placements(std::move(placements)) {
    restore_ordering_invariant();
}


figurine_placements_type::figurine_placements_type(
    std::vector<figurine_placement_type> const & placements
) : the_placements(placements) {
    restore_ordering_invariant();
}


figurine_placements_type::figurine_placements_type(
    std::initializer_list<figurine_placement_type> placements
) : the_placements(placements) {
    restore_ordering_invariant();
}


std::pair<bool, representation::bitboards_type>
figurine_placements_type::as_bitboards() const {
    bool overlapping{};
    representation::bitboards_type bitboards;
    for (std::size_t nth_placement{}; nth_placement != the_placements.size();
        ++nth_placement
    ) {
        overlapping |= not bitboards.place(the_placements[nth_placement]);
    }
    return std::make_pair(overlapping, bitboards);
}


std::pair<bool, epd::board_contents_type>
figurine_placements_type::as_board_contents() const {
    bool overlapping{};
    epd::board_contents_type board_contents;
    for (figurine_placement_type const placement: the_placements) {
        unsigned char const index{static_cast<unsigned char>(
         placement.coordinate().as_enum())};
        overlapping |= ('1' != board_contents[index]);
        board_contents.set_square_content(index,
         placement.figurine().as_char());
    }
    return std::make_pair(overlapping, board_contents);
}


logic_digits_type figurine_placements_type::as_logic_digits() const {
    logic_digits_type result;
    for (std::size_t i{}; i != the_placements.size(); ++i) {
        result.append(the_placements[i].as_logic_digits());
    }
    return result;
}


std::string figurine_placements_type::as_string() const {
    std::ostringstream out;
    out.put('[');
    for (std::size_t nth_placement{}; nth_placement != the_placements.size();
        ++nth_placement
    ) {
        if (0 != nth_placement) out << ", ";
        out << the_placements[nth_placement].as_string();
    }
    out.put(']');
    return out.str();
}


coordinates_type figurine_placements_type::coordinates() const {
    std::vector<coordinate_type> coordinates;
    for (figurine_placement_type const & placement: the_placements) {
        coordinates.push_back(placement.coordinate());
    }
    return coordinates_type{coordinates};
}


figurines_type figurine_placements_type::figurines() const {
    std::vector<figurine_type> figurines;
    std::for_each(the_placements.begin(), the_placements.end(),
     [&](figurine_placement_type const & figurine_placement) {
        figurines.push_back(figurine_placement.figurine());
    });
    return figurines_type{figurines};
}


material_signature_type figurine_placements_type::material_signature() const {
    std::optional<material_signature_type> signature_opt{
     material_signature_type::create_opt(figurines())};
    assert(signature_opt);
    return *signature_opt;
}


bool figurine_placements_type::contains(
    figurine_placement_type const figurine_placement
) const {
    return cend() != std::find(cbegin(), cend(), figurine_placement);
}


// This routine looks for a single instance of the old figurine placement.  If
// it is found, then the internal state of this object is updated to replace
// that old figurine placement with the new one in such a way that the ordering
// invariant is retained, and true is returned.  Otherwise, no change is made to
// the object state, and false is returned.
bool figurine_placements_type::replace(
    figurine_placement_type const remove,
    figurine_placement_type const add
) {
    auto iter{std::find(the_placements.begin(), the_placements.end(), remove)};
    if (iter == the_placements.end()) {
        return false;
    }
    *iter = add;
    restore_ordering_invariant();
    return true;
}


void figurine_placements_type::flip_a8h1s() {
    std::for_each(the_placements.begin(), the_placements.end(),
     [&](figurine_placement_type & figurine_placement) {
        figurine_placement.flip_a8h1();
    });
}


void figurine_placements_type::flip_colours() {
    std::for_each(the_placements.begin(), the_placements.end(),
     [&](figurine_placement_type & figurine_placement) {
        figurine_placement.flip_colour();
    });
}


void figurine_placements_type::flip_files() {
    std::for_each(the_placements.begin(), the_placements.end(),
     [&](figurine_placement_type & figurine_placement) {
        figurine_placement.flip_file();
    });
}


void figurine_placements_type::flip_ranks() {
    std::for_each(the_placements.begin(), the_placements.end(),
     [&](figurine_placement_type & figurine_placement) {
        figurine_placement.flip_rank();
    });
}


void figurine_placements_type::invert() {
    std::for_each(the_placements.begin(), the_placements.end(),
     [&](figurine_placement_type & figurine_placement) {
        figurine_placement.flip_colour();
        figurine_placement.flip_rank();
    });
}


void figurine_placements_type::restore_ordering_invariant() {
    std::sort(the_placements.begin(), the_placements.end());
}


std::vector<figurine_placement_type>::const_iterator
figurine_placements_type::black_cbegin() const {
    return std::find_if(the_placements.cbegin(), the_placements.cend(),
     [&](figurine_placement_type const & figurine_placement) {
        return black_king == figurine_placement.figurine();
    });
}


std::vector<figurine_placements_type>
figurine_placements_type::pawnless_symmetrical_placements() const {
    std::vector<figurine_placements_type> result(16, *this);
    for (std::size_t i{}; i != 16; ++i) {
        if (i & 0b0001) result[i].invert();
        if (i & 0b0010) result[i].flip_files();
        if (i & 0b0100) result[i].flip_ranks();
        if (i & 0b1000) result[i].flip_a8h1s();
        if (0 != i) std::sort(result[i].begin(), result[i].end());
    }
    return result;
}


std::vector<figurine_placements_type>
figurine_placements_type::pawnful_symmetrical_placements() const {
    std::vector<figurine_placements_type> result(4, *this);
    for (std::size_t i{}; i != 4; ++i) {
        if (i & 0b01) result[i].invert();
        if (i & 0b10) result[i].flip_files();
        if (0 != i) std::sort(result[i].begin(), result[i].end());
    }
    return result;
}


std::vector<figurine_placements_type>
figurine_placements_type::symmetrical_placements() const {
    material_signature_type const signature(material_signature());
    return signature.pawnless() ? pawnless_symmetrical_placements()
     : pawnful_symmetrical_placements();
}


figurine_placements_type figurine_placements_type::canonical_placement() const {
    std::vector<figurine_placements_type> symmetries{symmetrical_placements()};
    return *std::min_element(symmetries.begin(), symmetries.end());
}


void figurine_placements_type::canonicalize() {
    *this = canonical_placement();
}


bool operator==(
    figurine_placements_type const & lhs,
    figurine_placements_type const & rhs
) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}


bool operator!=(
    figurine_placements_type const & lhs,
    figurine_placements_type const & rhs
) {
    return not (lhs == rhs);
}


bool operator<(
    figurine_placements_type const & lhs,
    figurine_placements_type const & rhs
) {
    return std::lexicographical_compare(
     lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}


bool operator>(
    figurine_placements_type const & lhs,
    figurine_placements_type const & rhs
) {
    return not (rhs < lhs);
}


bool operator<=(
    figurine_placements_type const & lhs,
    figurine_placements_type const & rhs
) {
    return not (lhs > rhs);
}


bool operator>=(
    figurine_placements_type const & lhs,
    figurine_placements_type const & rhs
) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    figurine_placements_type const & figurine_placements
) {
    return out << figurine_placements.as_string();
}


template figurine_placements_type::figurine_placements_type(
    material_signature_type const &, std::vector<bool> &);
template figurine_placements_type::figurine_placements_type(
    material_signature_type const &, std::vector<unsigned char> &);
template figurine_placements_type::figurine_placements_type(
    material_signature_type const &, std::vector<unsigned short> &);
template figurine_placements_type::figurine_placements_type(
    material_signature_type const &, std::vector<unsigned int> &);
template figurine_placements_type::figurine_placements_type(
    material_signature_type const &, std::vector<unsigned long> &);
template figurine_placements_type::figurine_placements_type(
    material_signature_type const &, std::vector<unsigned long long> &);

template void figurine_placements_type::append(std::vector<bool> &) const;
template void figurine_placements_type::append(
    std::vector<unsigned char> &) const;
template void figurine_placements_type::append(
    std::vector<unsigned short> &) const;
template void figurine_placements_type::append(
    std::vector<unsigned int> &) const;
template void figurine_placements_type::append(
    std::vector<unsigned long> &) const;
template void figurine_placements_type::append(
    std::vector<unsigned long long> &) const;


} // namespace representation
