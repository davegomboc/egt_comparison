#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_PLACEMENTS_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_PLACEMENTS_TYPE_HPP


#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "epd/board_contents_type.hpp"

#include "representation/bitboards_type.hpp"
#include "representation/coordinate_type.hpp"
#include "representation/coordinates_type.hpp"
#include "representation/logic_digits_type.hpp"
#include "representation/figurine_placement_type.hpp"
#include "representation/material_signature_type.hpp"


namespace representation {


class figurine_placements_type {
public:
    figurine_placements_type & operator=(figurine_placements_type const &)
     = default;
    figurine_placements_type & operator=(figurine_placements_type &&) = default;
    figurine_placements_type(figurine_placements_type const &) = default;
    figurine_placements_type(figurine_placements_type &&) = default;
    ~figurine_placements_type() = default;

    // Does NOT order the placements, so that it can be used for iterating over
    // all permutations or for sampling from all permutations.
    figurine_placements_type(
        material_signature_type const & material_signature,
        coordinates_type const & coordinates);
    // Orders the placements.
    figurine_placements_type(epd::board_contents_type const & board_contents);
    // Orders the placements.
    figurine_placements_type(
        std::vector<figurine_placement_type> && placements);
    // Orders the placements.
    figurine_placements_type(
        std::vector<figurine_placement_type> const & placements);
    // Orders the placements.
    figurine_placements_type(
        std::initializer_list<figurine_placement_type> placements);
    // Orders the placements.
    figurine_placements_type(
        material_signature_type const & material_signature,
        std::size_t const position_id
    ) {
        [[maybe_unused]] cube_schema_type const cube_schema(
         material_signature.cube_schema());
        assert(position_id < cube_schema.position_count());
        std::size_t consumable_position_id(position_id);
        for (std::size_t nth_placement{material_signature.size()};
         0 != nth_placement; --nth_placement) {
            the_placements.emplace_back(figurine_placement_type(
             material_signature[nth_placement - 1],
             coordinate_type(consumable_position_id)));
        }
        // Only the bit indicating which player is to move should remain.
        assert(0b1 == (0b1 | consumable_position_id));
        std::reverse(the_placements.begin(), the_placements.end());
    }
    // Orders the placements.
    template <typename uint_type>
    figurine_placements_type(
        material_signature_type const & material_signature,
        std::vector<uint_type> & vector_id
    ) {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
        for (std::size_t nth_placement{material_signature.size()};
         0 != nth_placement; --nth_placement) {
            the_placements.emplace_back(figurine_placement_type(
             material_signature[nth_placement - 1],
             coordinate_type(vector_id)));
        }
        std::reverse(the_placements.begin(), the_placements.end());
    }

    std::vector<figurine_placement_type>::const_iterator cbegin() const {
        return the_placements.cbegin();
    }
    std::vector<figurine_placement_type>::const_iterator begin() const {
        return the_placements.begin();
    }
    std::vector<figurine_placement_type>::iterator begin() {
        return the_placements.begin();
    }

    std::vector<figurine_placement_type>::const_iterator cend() const {
        return the_placements.cend();
    }
    std::vector<figurine_placement_type>::const_iterator end() const {
        return the_placements.end();
    }
    std::vector<figurine_placement_type>::iterator end() {
        return the_placements.end();
    }

    coordinates_type coordinates() const;
    figurines_type figurines() const;
    material_signature_type material_signature() const;

    std::size_t input_bit_pair_count() const {
        return 1 + 6 * the_placements.size();
    }

    bool contains(figurine_placement_type const figurine_placement) const;
    bool replace(
        figurine_placement_type const remove,
        figurine_placement_type const add);

    std::string as_string() const;
    std::pair<bool, representation::bitboards_type> as_bitboards() const;
    std::pair<bool, epd::board_contents_type> as_board_contents() const;
    logic_digits_type as_logic_digits() const;

    void append(std::size_t & position_id) const {
        for (figurine_placement_type const & placement: the_placements) {
            placement.append(position_id);
        }
    }

    template <typename uint_type>
    void append(std::vector<uint_type> & vector_id) const {
        static_assert(std::is_integral_v<uint_type>,
         "Template argument must be integral.");
        static_assert(std::is_unsigned_v<uint_type>,
         "Template argument must be unsigned.");
        for (figurine_placement_type const & placement: the_placements) {
            placement.append(vector_id);
        }
    }

    std::vector<figurine_placements_type> symmetrical_placements() const;
    figurine_placements_type canonical_placement() const;
    void canonicalize();

    // These five functions don't themselves maintain the invariant
    // that the_placements must be sorted according to operator<.
    void flip_a8h1s();
    void flip_colours();
    void flip_files();
    void flip_ranks();
    void invert();
    void restore_ordering_invariant();

private:
    std::vector<figurine_placement_type>::const_iterator black_cbegin() const;

    std::vector<figurine_placements_type>
     pawnless_symmetrical_placements() const;

    std::vector<figurine_placements_type>
     pawnful_symmetrical_placements() const;

    std::vector<figurine_placement_type> the_placements;
};


bool operator==(
 figurine_placements_type const & lhs, figurine_placements_type const & rhs);
bool operator!=(
 figurine_placements_type const & lhs, figurine_placements_type const & rhs);
bool operator<(
 figurine_placements_type const & lhs, figurine_placements_type const & rhs);
bool operator>(
 figurine_placements_type const & lhs, figurine_placements_type const & rhs);
bool operator<=(
 figurine_placements_type const & lhs, figurine_placements_type const & rhs);
bool operator>=(
 figurine_placements_type const & lhs, figurine_placements_type const & rhs);


std::ostream & operator<<(
    std::ostream & out,
    figurine_placements_type const & placements);


} // namespace representation


namespace std {


template <> class hash<representation::figurine_placements_type> {
public:
    std::size_t operator()(
        representation::figurine_placements_type const & figurine_placements
    ) const {
        return hash<string>()(figurine_placements.as_string());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_PLACEMENTS_TYPE_HPP
