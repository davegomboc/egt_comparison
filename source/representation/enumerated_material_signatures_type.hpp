#if !defined(EGT_COMPARISON_REPRESENTATION_ENUMERATED_MATERIAL_SIGNATURES_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_ENUMERATED_MATERIAL_SIGNATURES_TYPE_HPP


#include <array>
#include <vector>

#include "representation/figurines_type.hpp"
#include "representation/material_signature_type.hpp"


namespace representation {


static constexpr unsigned int the_minimum_figurine_count{2};
static constexpr unsigned int the_maximum_figurine_count{9};


class enumerated_material_signatures_type {
public:
    enumerated_material_signatures_type(
        enumerated_material_signatures_type const &) = default;
    enumerated_material_signatures_type(
        enumerated_material_signatures_type &&) = default;
    enumerated_material_signatures_type & operator=(
        enumerated_material_signatures_type const &) = default;
    enumerated_material_signatures_type & operator=(
        enumerated_material_signatures_type &&) = default;
    ~enumerated_material_signatures_type() = default;
    enumerated_material_signatures_type(unsigned int highest_figurine_count);

    std::size_t material_signature_count(unsigned int figurine_count) const;

    representation::material_signature_type material_signature(
        unsigned int figurine_count,
        std::size_t material_signature_index) const;

    std::vector<representation::material_signature_type> const &
     material_signatures(unsigned int figurine_count) const;

    std::vector<representation::material_signature_type>
     material_signatures() const;

private:
    static std::array<
        std::vector<representation::figurines_type>,
        the_maximum_figurine_count
    > determine_single_colour_figurine_combinations(
        unsigned int highest_figurine_count);
    static std::array<
        std::vector<representation::material_signature_type>,
        the_maximum_figurine_count + 1
    > determine_double_colour_figurine_combinations(
        unsigned int highest_figurine_count,
        std::array<
            std::vector<representation::figurines_type>,
            the_maximum_figurine_count
        > const & single_colour_combos);

    unsigned int the_highest_figurine_count;

    std::array<
        std::vector<representation::figurines_type>,
        the_maximum_figurine_count
    > the_single_colour_figurine_combinations;

    std::array<
        std::vector<representation::material_signature_type>,
        the_maximum_figurine_count + 1
    > the_double_colour_figurine_combinations;
};


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_ENUMERATED_MATERIAL_SIGNATURES_TYPE_HPP
