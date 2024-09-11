#if !defined(EGT_COMPARISON_REPRESENTATION_COORDINATES_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_COORDINATES_TYPE_HPP


#include <algorithm>
#include <cassert>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

#include "representation/coordinate_type.hpp"


namespace representation {


class coordinates_type {
public:
    coordinates_type & operator=(coordinates_type const &) = default;
    coordinates_type & operator=(coordinates_type &&) = default;
    coordinates_type(coordinates_type const &) = default;
    coordinates_type(coordinates_type &&) = default;
    ~coordinates_type() = default;

    coordinates_type(std::vector<coordinate_type> const & coordinates)
      : the_coordinates(coordinates) {
        assert(the_coordinates.size() < 10);
    }
    coordinates_type(std::vector<coordinate_type> && coordinates)
      : the_coordinates(std::move(coordinates)) {
        assert(the_coordinates.size() < 10);
    }
    coordinates_type(std::initializer_list<coordinate_type> const coordinates)
     : the_coordinates(coordinates) {
        assert(the_coordinates.size() < 10);
    }
    coordinates_type(std::size_t figurine_count, std::size_t index);

    void push_back(coordinate_type const &);
    void push_back(coordinate_type &&);

    bool empty() const {
        return the_coordinates.empty();
    }

    std::size_t size() const {
        return the_coordinates.size();
    }

    bool overlapping() const {
        std::vector<coordinate_type> copy{the_coordinates};
        std::sort(copy.begin(), copy.end());
        return copy.end() != std::unique(copy.begin(), copy.end());
    }

    auto cbegin() const {
        return the_coordinates.cbegin();
    }
    auto begin() const {
        return the_coordinates.cbegin();
    }
    auto begin() {
        return the_coordinates.begin();
    }

    auto cend() const {
        return the_coordinates.cbegin();
    }
    auto end() const {
        return the_coordinates.cbegin();
    }
    auto end() {
        return the_coordinates.begin();
    }

    coordinate_type operator[](std::size_t const index) const {
        return the_coordinates[index];
    }

    std::string as_string() const;

    bool next();
    coordinates_type & operator++();

    std::size_t as_index() const {
        std::size_t result{};
        for (unsigned nth_place{}; nth_place != size(); ++nth_place) {
            unsigned char const base_64_digit{static_cast<unsigned char>(
             the_coordinates[size() - nth_place - 1].as_enum())};
            result += static_cast<std::size_t>(base_64_digit) <<
             (6 * nth_place);
        }
        return result;
    }

    void flip_file();
    void flip_rank();
    void flip_a8h1();

private:
    std::vector<coordinate_type> the_coordinates;
};


bool operator==(coordinates_type const &, coordinates_type const &);
bool operator!=(coordinates_type const &, coordinates_type const &);
bool operator<(coordinates_type const &, coordinates_type const &);
bool operator>(coordinates_type const &, coordinates_type const &);
bool operator<=(coordinates_type const &, coordinates_type const &);
bool operator>=(coordinates_type const &, coordinates_type const &);


std::ostream & operator<<(
    std::ostream & out,
    coordinates_type const & coordinates);


} // namespace representation


namespace std {


template <> class hash<representation::coordinates_type> {
public:
    std::size_t operator()(
        representation::coordinates_type const & coordinates
    ) const {
        return hash<string>()(coordinates.as_string());
    }
};


} // namespace std


#endif // EGT_COMPARISON_REPRESENTATION_COORDINATES_TYPE_HPP
