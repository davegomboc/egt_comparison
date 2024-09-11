#if !defined(EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPES_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPES_TYPE_HPP


#include <algorithm>
#include <initializer_list>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <vector>

#include "representation/figurine_shape_type.hpp"


namespace representation {


class figurine_shapes_type {
public:
    figurine_shapes_type & operator=(figurine_shapes_type const &) = default;
    figurine_shapes_type & operator=(figurine_shapes_type &&) = default;
    figurine_shapes_type(figurine_shapes_type const &) = default;
    figurine_shapes_type(figurine_shapes_type &&) = default;
    ~figurine_shapes_type() = default;

    figurine_shapes_type() {}
    figurine_shapes_type(std::vector<figurine_shape_type> const & shapes)
      : the_shapes(shapes) {}
    figurine_shapes_type(std::vector<figurine_shape_type> && shapes)
      : the_shapes(std::move(shapes)) {}
    figurine_shapes_type(
        std::initializer_list<figurine_shape_type> const shapes
    ) : the_shapes(shapes) {}

    static std::optional<figurine_shapes_type> create_opt(
        std::string_view const shapes);

    bool contains(figurine_shape_type const shape) const {
        return the_shapes.cend() != std::find(the_shapes.cbegin(),
         the_shapes.cend(), shape);
    }

    bool empty() const {
        return the_shapes.empty();
    }

    std::size_t size() const {
        return the_shapes.size();
    }

    std::string as_string() const;

    figurine_shape_type const & operator[](std::size_t index) const;
    figurine_shape_type & operator[](std::size_t index);

    bool valid_material_side() const;

    void push_back(figurine_shape_type const &);
    void push_back(figurine_shape_type &&);

    auto cbegin() const { return the_shapes.cbegin(); }
    auto begin() const { return the_shapes.cbegin(); }
    auto begin() { return the_shapes.begin(); }

    auto cend() const { return the_shapes.cend(); }
    auto end() const { return the_shapes.cend(); }
    auto end() { return the_shapes.end(); }

private:
    std::vector<figurine_shape_type> the_shapes;
};


bool operator==(figurine_shapes_type const &, figurine_shapes_type const &);
bool operator!=(figurine_shapes_type const &, figurine_shapes_type const &);
bool operator<(figurine_shapes_type const &, figurine_shapes_type const &);
bool operator>(figurine_shapes_type const &, figurine_shapes_type const &);
bool operator<=(figurine_shapes_type const &, figurine_shapes_type const &);
bool operator>=(figurine_shapes_type const &, figurine_shapes_type const &);


std::ostream & operator<<(
    std::ostream & out,
    figurine_shapes_type const & figurine_shapes_type);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_FIGURINE_SHAPES_TYPE_HPP
