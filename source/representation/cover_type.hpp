#if !defined(EGT_COMPARISON_REPRESENTATION_COVER_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_COVER_TYPE_HPP


#include <algorithm>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iosfwd>
#include <vector>

#include "hex_display.hpp"

#include "probe_report_type.hpp"

#include "representation/cover_enum.hpp"
#include "representation/cube_type.hpp"
#include "representation/logic_digits_type.hpp"


namespace representation {


class cover_type {

public:
    cover_type(cover_type const &) = default;
    cover_type(cover_type &&) = default;
    cover_type & operator=(cover_type const &) = default;
    cover_type & operator=(cover_type &&) = default;
    ~cover_type() = default;
    cover_type() = default;

    cover_type(
        std::size_t const cube_count,
        std::size_t const per_cube_bit_count
    ) : the_cubes(cube_count, cube_type(per_cube_bit_count)) {}

    void read(std::istream & in);

    static std::unique_ptr<cover_type> create_ptr(
     std::istream & in,
     std::size_t cube_count,
     std::size_t per_cube_bit_count);

    cube_type const & operator[](std::size_t const nth_cube) const {
        assert(nth_cube < the_cubes.size());
        return the_cubes[nth_cube];
    }
    cube_type & operator[](std::size_t const nth_cube) {
        assert(nth_cube < the_cubes.size());
        return the_cubes[nth_cube];
    }
    auto data() const noexcept { return the_cubes.data(); }
    auto data() noexcept { return the_cubes.data(); }
    auto front() const { return the_cubes.front(); }
    auto front() { return the_cubes.front(); }
    auto back() const { return the_cubes.back(); }
    auto back() { return the_cubes.back(); }

    auto cbegin() const noexcept { return the_cubes.cbegin(); }
    auto begin() const noexcept { return the_cubes.begin(); }
    auto begin() noexcept { return the_cubes.begin(); }
    auto cend() const noexcept { return the_cubes.cend(); }
    auto end() const noexcept { return the_cubes.end(); }
    auto end() noexcept { return the_cubes.end(); }
    auto crbegin() const noexcept { return the_cubes.crbegin(); }
    auto rbegin() const noexcept { return the_cubes.rbegin(); }
    auto rbegin() noexcept { return the_cubes.rbegin(); }
    auto crend() const noexcept { return the_cubes.crend(); }
    auto rend() const noexcept { return the_cubes.rend(); }
    auto rend() noexcept { return the_cubes.rend(); }

    auto empty() const noexcept { return the_cubes.empty(); }
    auto size() const noexcept { return the_cubes.size(); }
    auto max_size() const noexcept { return the_cubes.max_size(); }
    void reserve(std::size_t new_capacity) { the_cubes.reserve(new_capacity); }
    auto capacity() const noexcept { return the_cubes.capacity(); }
    void shrink_to_fit() { the_cubes.shrink_to_fit(); }

    void clear() noexcept { the_cubes.clear(); }
    void pop_back() { the_cubes.pop_back(); }
    void push_back(cube_type const & value) { the_cubes.push_back(value); }
    void push_back(cube_type && value) { the_cubes.push_back(value); }

    void swap(cover_type & other) {
        using std::swap;
        swap(the_cubes, other.the_cubes);
    }

    bool is_sorted() const {
        return std::is_sorted(the_cubes.cbegin(), the_cubes.cend());
    }

    void sort() {
        std::sort(the_cubes.begin(), the_cubes.end());
    }

    std::vector<cube_type>::const_iterator find_containing_cube(
        cube_type const & contained_cube
    ) const;

    void print(
        std::ostream & out,
        std::function<void(std::ostream &, cube_type const &)> cube_printer
    ) const {
        std::size_t const cube_count{the_cubes.size()};
        out << "The cover consists of " << cube_count << " cube";
        if (1 != cube_count) {
            out << 's';
        }
        out << ".\n";
        for (std::size_t nth_cube{}; nth_cube != cube_count; ++nth_cube) {
            out << '[' << std::dec << std::noshowbase << std::internal
             << std::setfill(' ') << std::setw(4) << nth_cube << "] ";
            cube_printer(out, the_cubes[nth_cube]);
            out << '\n';
        }
        out << std::flush;
    }

    void write(std::ostream & out) const {
        assert(is_sorted());
        for (std::size_t nth_cube{}; nth_cube != size(); ++nth_cube) {
            the_cubes[nth_cube].write(out);
        }
    }

private:
    std::vector<cube_type> the_cubes;
};


cube_type prepare_cube_for_cover(
    logic_digits_type const & logic_digits,
    cover_enum cover,
    probe_report_type const wpr_probe_report);


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_COVER_TYPE_HPP
