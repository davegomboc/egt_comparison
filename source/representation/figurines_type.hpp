#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINES_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINES_TYPE_HPP


#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "epd/board_contents_type.hpp"

#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_shapes_type.hpp"
#include "representation/figurine_type.hpp"


namespace representation {


class figurines_type {
public:
    figurines_type & operator=(figurines_type const &) = default;
    figurines_type & operator=(figurines_type &&) = default;
    figurines_type(figurines_type const &) = default;
    figurines_type(figurines_type &&) = default;
    ~figurines_type() = default;

    figurines_type() {}
    figurines_type(std::vector<figurine_type> const & figurines)
      : the_figurines(figurines) {}
    figurines_type(std::vector<figurine_type> && figurines)
      : the_figurines(std::move(figurines)) {}
    figurines_type(std::initializer_list<figurine_type> const figurines)
      : figurines_type(std::vector<figurine_type>(figurines)) {}
    figurines_type(
        figurine_colour_type colour,
        figurine_shapes_type const & shapes);
    figurines_type(
        figurine_shapes_type const & white_shapes,
        figurine_shapes_type const & black_shapes);
    figurines_type(epd::board_contents_type const & board_contents);

    static std::optional<figurines_type> create_opt(
        std::string_view const figurines);

    bool empty() const {
        return the_figurines.empty();
    }

    std::size_t size() const {
        return the_figurines.size();
    }

    std::string as_string() const;

    figurine_shapes_type shapes(figurine_colour_type colour) const;

    figurine_type const & operator[](std::size_t index) const;
    figurine_type & operator[](std::size_t index);

    bool en_passant_can_happen() const;
    bool valid_material_balance() const;

    void flip_colours();

    void push_back(figurine_type const &);
    void push_back(figurine_type &&);

    auto cbegin() const { return the_figurines.cbegin(); }
    auto begin() const { return the_figurines.cbegin(); }
    auto begin() { return the_figurines.begin(); }

    auto cend() const { return the_figurines.cend(); }
    auto end() const { return the_figurines.cend(); }
    auto end() { return the_figurines.end(); }

private:
    std::vector<figurine_type> the_figurines;
};


bool operator==(figurines_type const & lhs, figurines_type const & rhs);
bool operator!=(figurines_type const & lhs, figurines_type const & rhs);
bool operator<(figurines_type const & lhs, figurines_type const & rhs);
bool operator>(figurines_type const & lhs, figurines_type const & rhs);
bool operator<=(figurines_type const & lhs, figurines_type const & rhs);
bool operator>=(figurines_type const & lhs, figurines_type const & rhs);


std::ostream & operator<<(
    std::ostream & out,
    figurines_type const & figurines);


} // namespace representation


namespace std {


template <> class hash<representation::figurines_type> {
public:
    std::size_t operator()(
        representation::figurines_type const & figurines
    ) const {
        return hash<string>()(figurines.as_string());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINES_TYPE_HPP
