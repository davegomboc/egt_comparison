#if !defined(EGT_COMPARISON_REPRESENTATION_MATERIAL_SIDE_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_MATERIAL_SIDE_TYPE_HPP


#include <functional>
#include <initializer_list>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_shape_type.hpp"
#include "representation/figurine_shapes_type.hpp"


namespace representation {


class material_side_type {
public:
    material_side_type & operator=(material_side_type const &) = default;
    material_side_type & operator=(material_side_type &&) = default;
    material_side_type(material_side_type const &) = default;
    material_side_type(material_side_type &&) = default;
    ~material_side_type() = default;

    material_side_type()
      : material_side_type(figurine_shapes_type{king}) {}

    static std::optional<material_side_type> create_opt(
        figurine_shapes_type const &);
    static std::optional<material_side_type> create_opt(
        figurine_shapes_type &&);
    static std::optional<material_side_type> create_opt(
        std::initializer_list<figurine_shape_type>);
    static std::optional<material_side_type> create_opt(std::string_view);

    std::size_t size() const { return the_material_side.size(); }

    figurine_shapes_type const & as_figurine_shapes() const {
        return the_material_side;
    }

    std::string as_string(figurine_colour_type colour = white) const;

    bool castling_can_happen() const;
    bool pawnful() const;
    bool pawnless() const;

    figurine_shape_type const & operator[](std::size_t index) const;
    figurine_shape_type & operator[](std::size_t index);

    std::vector<figurine_shape_type>::const_iterator cbegin() const {
        return the_material_side.cbegin();
    }
    std::vector<figurine_shape_type>::const_iterator begin() const {
        return the_material_side.cbegin();
    }
    std::vector<figurine_shape_type>::iterator begin() {
        return the_material_side.begin();
    }

    std::vector<figurine_shape_type>::const_iterator cend() const {
        return the_material_side.cend();
    }
    std::vector<figurine_shape_type>::const_iterator end() const {
        return the_material_side.cend();
    }
    std::vector<figurine_shape_type>::iterator end() {
        return the_material_side.end();
    }


private:
    material_side_type(figurine_shapes_type const &);

    figurine_shapes_type the_material_side;
};


bool operator==(material_side_type const & lhs, material_side_type const & rhs);
bool operator!=(material_side_type const & lhs, material_side_type const & rhs);
bool operator<(material_side_type const & lhs, material_side_type const & rhs);
bool operator>(material_side_type const & lhs, material_side_type const & rhs);
bool operator<=(material_side_type const & lhs, material_side_type const & rhs);
bool operator>=(material_side_type const & lhs, material_side_type const & rhs);


std::ostream & operator<<(
    std::ostream & out,
    material_side_type const & material_side);


} // namespace representation


namespace std {


template <> class hash<representation::material_side_type> {
public:
    std::size_t operator()(
        representation::material_side_type const & material_side
    ) const {
        return hash<string>()(material_side.as_string());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_MATERIAL_SIDE_TYPE_HPP
