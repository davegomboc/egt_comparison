#include "enumerated_material_signatures_type.hpp"


#include <algorithm>
#include <cassert>
#include <iterator>
#include <set>

#include "representation/figurine_enum.hpp"


namespace representation {


enumerated_material_signatures_type::enumerated_material_signatures_type(
    unsigned int const highest_figurine_count
) : the_highest_figurine_count(highest_figurine_count) {
    assert((the_minimum_figurine_count <= highest_figurine_count)
     and (highest_figurine_count <= the_maximum_figurine_count));
    the_single_colour_figurine_combinations =
     determine_single_colour_figurine_combinations(highest_figurine_count);
    the_double_colour_figurine_combinations =
     determine_double_colour_figurine_combinations(highest_figurine_count,
     the_single_colour_figurine_combinations);
}


std::size_t enumerated_material_signatures_type::material_signature_count(
    unsigned int const figurine_count
) const {
    if (figurine_count > the_highest_figurine_count) return 0;
    return the_double_colour_figurine_combinations[figurine_count].size();
}


material_signature_type enumerated_material_signatures_type::material_signature(
    unsigned int const figurine_count,
    std::size_t const material_signature_index
) const {
    assert(figurine_count <= the_highest_figurine_count);
    assert(material_signature_index < material_signature_count(figurine_count));
    assert(the_double_colour_figurine_combinations[figurine_count].size()
     == material_signature_count(figurine_count));
    return the_double_colour_figurine_combinations[figurine_count]
     [material_signature_index];
}

std::vector<material_signature_type> const &
enumerated_material_signatures_type::material_signatures(
    unsigned int figurine_count
) const {
    assert(figurine_count <= the_highest_figurine_count);
    return the_double_colour_figurine_combinations[figurine_count];
}


std::vector<material_signature_type>
enumerated_material_signatures_type::material_signatures() const {
    std::vector<material_signature_type> result;
    for (unsigned int figurine_count{the_minimum_figurine_count};
     figurine_count <= the_highest_figurine_count; ++figurine_count) {
        for (material_signature_type const & signature:
         material_signatures(figurine_count)) {
            result.push_back(signature);
        }
    }
    return result;
}


std::array<std::vector<figurines_type>, the_maximum_figurine_count>
 enumerated_material_signatures_type::
 determine_single_colour_figurine_combinations(
    unsigned int const highest_figurine_count
) {
    std::array<std::set<figurines_type>, the_maximum_figurine_count>
     single_colour_combos;
    figurines_type just_king{white_king};
    single_colour_combos[1] = std::set<figurines_type>{just_king};
    for (unsigned int i{1}; i != highest_figurine_count; ++i) {
        std::for_each(single_colour_combos[i - 1].cbegin(),
         single_colour_combos[i - 1].cend(),
         [&](figurines_type const & existing_figurines) {
            std::array<figurines_type, 5> next;
            for (unsigned int j{}; j != 5; ++j) {
                next[j] = existing_figurines;
                next[j].push_back(figurine_type{static_cast<figurine_enum>(
                 2 + j)});
                std::sort(next[j].begin(), next[j].end());
            }
            std::for_each(next.cbegin(), next.cend(),
             [&](figurines_type const & add_me) {
                single_colour_combos[i].insert(add_me);
             });
         });
    }
    std::array<std::vector<figurines_type>, the_maximum_figurine_count> result;
    for (unsigned int i{1}; i <= highest_figurine_count; ++i) {
        result[i] = std::vector(single_colour_combos[i].cbegin(),
         single_colour_combos[i].cend());
    }
    return result;
}


std::array<std::vector<material_signature_type>, the_maximum_figurine_count + 1>
 enumerated_material_signatures_type::
 determine_double_colour_figurine_combinations(
    unsigned int const highest_figurine_count,
    std::array<std::vector<figurines_type>, the_maximum_figurine_count> const &
     single_colour_combos
) {
    std::array<
        std::set<material_signature_type>,
        the_maximum_figurine_count + 1
    > double_colour_combos;
    for (unsigned int level{2}; level != highest_figurine_count + 1; ++level) {
        for (auto white_figurine_count{level - 1u},
         black_figurine_count{level - white_figurine_count};
         black_figurine_count <= white_figurine_count;
         --white_figurine_count, ++black_figurine_count) {
            std::for_each(single_colour_combos[white_figurine_count].cbegin(),
             single_colour_combos[white_figurine_count].cend(),
             [&](figurines_type const & white_figurines) {
                std::for_each(
                 single_colour_combos[black_figurine_count].cbegin(),
                 single_colour_combos[black_figurine_count].cend(),
                 [&](figurines_type const & black_figurines) {
                    // >=, because K < Q < R < B < N < P in figurine_enum.
                    if ((white_figurine_count != black_figurine_count) or
                     (black_figurines >= white_figurines)) {
                        figurines_type potential_material_signature{
                         black_figurines};
                        potential_material_signature.flip_colours();
                        for (std::size_t i{}; i != white_figurines.size();
                         ++i) {
                            potential_material_signature.push_back(
                             white_figurines[i]);
                        }
                        auto const material_signature_opt{
                         material_signature_type::create_opt(
                         potential_material_signature)};
                        assert(material_signature_opt);
                        double_colour_combos[level].insert(
                         *material_signature_opt);
                    }
                });
            });
        }
    }
    std::array<
        std::vector<material_signature_type>,
        the_maximum_figurine_count + 1
    > result;
    for (unsigned int level{2}; level <= highest_figurine_count; ++level) {
        result[level] = std::vector(double_colour_combos[level].cbegin(),
         double_colour_combos[level].cend());
    }
    return result;
}


} // namespace representation
