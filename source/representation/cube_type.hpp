#if !defined(EGT_COMPARISON_REPRESENTATION_CUBE_TYPE_HPP)
#define EGT_COMPARISON_REPRESENTATION_CUBE_TYPE_HPP


#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>

#include "bit_array_display.hpp"
#include "bit_indexing_and_sizing.hpp"
#include "bit_region_type.hpp"
#include "bit_region_schema_type.hpp"
#include "default_init_type.hpp"
#include "hex_display.hpp"

#include "representation/logic_digit_type.hpp"


namespace representation {


class cube_type {

public:
    using uint_type = std::uint64_t;

    cube_type & operator=(cube_type const & other) {
        the_bit_count = other.the_bit_count;
        the_uint64s = other.the_uint64s;
        return *this;
    }
    cube_type & operator=(cube_type &&) = default;
    cube_type(cube_type const & other)
      : the_bit_count(other.the_bit_count),
        the_uint64s(other.the_uint64s) {}
    cube_type(cube_type &&) = default;
    ~cube_type() = default;

    explicit cube_type(std::size_t const bit_count)
      : the_bit_count(bit_count),
        the_uint64s(required<uint_type>(bit_count))
    {
        assert(required<uint_type>(bit_count) <= the_uint64s.size());
        // While we don't want to completely initialize each element of
        // the_uint64s, it is convenient to clear all bits in the last one now:
        // subsequently, no unused high bit should ever be set to one, so if it
        // happens, it indicates a problem.
        if (0 < bit_count) {
            the_uint64s[the_uint64s.size() - 1] = 0;
        }
    }

    explicit cube_type(
        std::initializer_list<bool> init
    ) : cube_type(init.size()) {
        std::size_t bit_n{};
        auto current_iter{init.begin()};
        while (current_iter != init.end()) {
            if (*current_iter) {
                set(bit_n);
            }
            ++current_iter;
            ++bit_n;
        }
    }

    cube_type(
        std::size_t const bit_count,
        std::initializer_list<std::uint64_t> init
    ) : the_bit_count(bit_count),
        the_uint64s(init) {}

    std::size_t bit_count() const {
        return the_bit_count;
    }

    bit_region_type<uint_type const> const_region() const {
        return bit_region_type<uint_type const>(the_bit_count,
         the_uint64s.data());
    }

    bit_region_type<uint_type> region() {
        return bit_region_type<uint_type>(the_bit_count, the_uint64s.data());
    }

    bool empty() const {
        return the_uint64s.empty();
    }

    std::size_t bit_capacity() const {
        return bits_per<std::uint64_t> * the_uint64s.size();
    }

    auto const & uint64s() const { return the_uint64s; }

    void debug_print(std::ostream & out) const {
        const_region().debug_print(out);
    }

    void probe_debug_print(std::ostream & out) const {
        const_region().probe_debug_print(out);
    }

    void print_as_hex(std::ostream & out) const {
        print_hex(out, the_uint64s.data(),
         the_uint64s.data() + the_uint64s.size());
    }

    void print_as_hex_chars(std::ostream & out) const {
        print_hex<std::uint8_t>(out,
         reinterpret_cast<std::uint8_t const *>(the_uint64s.data()),
         reinterpret_cast<std::uint8_t const *>(
         the_uint64s.data() + the_uint64s.size()));
    }

    void write(std::ostream & out) const {
        out.write(reinterpret_cast<char const *>(the_uint64s.data()),
         static_cast<std::streamsize>(
         the_uint64s.size() * sizeof(std::uint64_t)));
    }

    bool operator[](std::size_t const bit_n) const {
        assert(required<uint_type>(bit_n) <= the_uint64s.size());
        std::size_t const which_uint{which<uint_type>(bit_n)};
        uint_type const which_bit{one_hot_of<uint_type>(bit_n)};
        return 0 != (the_uint64s[which_uint] & which_bit);
    }

    void flip(std::size_t const bit_n) {
        assert(required<uint_type>(bit_n) <= the_uint64s.size());
        std::size_t const which_uint{which<uint_type>(bit_n)};
        uint_type const which_bit{one_hot_of<uint_type>(bit_n)};
        the_uint64s[which_uint] ^= which_bit;
    }

    void reset(std::size_t const bit_n) {
        set(bit_n, false);
    }

    void set(std::size_t const bit_n, bool const value = true) {
        assert(required<uint_type>(bit_n) <= the_uint64s.size());
        std::size_t const which_uint{which<uint_type>(bit_n)};
        uint_type const which_bit{one_hot_of<uint_type>(bit_n)};
        if (value) {
            the_uint64s[which_uint] |= which_bit;
        } else {
            the_uint64s[which_uint] &= invert(which_bit);
        }
    }

    cube_type & operator|=(cube_type const & other) {
        assert(other.uint64s().size() == uint64s().size());
        for (std::size_t i{}; i != other.uint64s().size(); ++i) {
            the_uint64s[i] |= other.uint64s()[i];
        }
        return *this;
    }

    cube_type & operator&=(cube_type const & other) {
        assert(other.uint64s().size() == uint64s().size());
        for (std::size_t i{}; i != other.uint64s().size(); ++i) {
            the_uint64s[i] &= other.uint64s()[i];
        }
        return *this;
    }

    cube_type & operator^=(cube_type const & other) {
        assert(other.uint64s().size() == uint64s().size());
        for (std::size_t i{}; i != other.uint64s().size(); ++i) {
            the_uint64s[i] ^= other.uint64s()[i];
        }
        return *this;
    }

    cube_type operator~() const {
        cube_type result(*this);
        std::size_t const limit{result.uint64s().size()};
        for (std::size_t i{}; i != limit; ++i) {
            result.the_uint64s[i] = ~the_uint64s[i];
        }
        return result;
    }

    void print(
        std::ostream & out,
        bit_region_schema_type const & schema,
        std::string_view const output_map) const;

    std::string as_string(
        bit_region_schema_type const & schema,
        std::string_view const output_map) const;

    void read(std::istream & in) {
        assert(in.good());
        std::size_t const chars_to_read{
         required<std::uint64_t>(bit_count()) * sizeof(std::uint64_t)};
        assert(chars_to_read < std::numeric_limits<std::streamsize>::max());
        in.read(reinterpret_cast<char *>(the_uint64s.data()),
         static_cast<std::streamsize>(chars_to_read));
        assert(in.good());
    }

private:
    std::size_t the_bit_count;
    std::vector<std::uint64_t, default_init_type<std::uint64_t>> the_uint64s;
};


inline cube_type operator|(cube_type const & lhs, cube_type const & rhs) {
    assert(lhs.uint64s().size() == rhs.uint64s().size());
    cube_type result{lhs};
    result |= rhs;
    return result;
}

inline cube_type operator|(cube_type && lhs, cube_type const & rhs) {
    assert(lhs.uint64s().size() == rhs.uint64s().size());
    lhs |= rhs;
    return std::move(lhs);
}

inline cube_type operator|(cube_type const & lhs, cube_type && rhs) {
    return operator|(rhs, lhs);
}

inline cube_type operator|(cube_type && lhs, cube_type && rhs) {
    return operator|(lhs, static_cast<cube_type const &>(rhs));
}

inline cube_type operator&(cube_type const & lhs, cube_type const & rhs) {
    assert(lhs.uint64s().size() == rhs.uint64s().size());
    cube_type result{lhs};
    result &= rhs;
    return result;
}

inline cube_type operator&(cube_type && lhs, cube_type const & rhs) {
    assert(lhs.uint64s().size() == rhs.uint64s().size());
    lhs &= rhs;
    return std::move(lhs);
}

inline cube_type operator&(cube_type const & lhs, cube_type && rhs) {
    return operator&(rhs, lhs);
}

inline cube_type operator&(cube_type && lhs, cube_type && rhs) {
    return operator&(lhs, static_cast<cube_type const &>(rhs));
}

inline cube_type operator^(cube_type const & lhs, cube_type const & rhs) {
    assert(lhs.uint64s().size() == rhs.uint64s().size());
    cube_type result{lhs};
    result ^= rhs;
    return result;
}

inline cube_type operator^(cube_type && lhs, cube_type const & rhs) {
    lhs ^= rhs;
    return std::move(lhs);
}

inline cube_type operator^(cube_type const & lhs, cube_type && rhs) {
    return operator^(rhs, lhs);
}

inline cube_type operator^(cube_type && lhs, cube_type && rhs) {
    return operator^(lhs, static_cast<cube_type const &>(rhs));
}

inline bool operator==(cube_type const & lhs, cube_type const & rhs) {
    return lhs.const_region() == rhs.const_region();
}

inline bool operator!=(cube_type const & lhs, cube_type const & rhs) {
    return not operator==(lhs, rhs);
}

inline bool operator<(cube_type const & lhs, cube_type const & rhs) {
    std::size_t const size{lhs.uint64s().size()};
    if (size < rhs.uint64s().size()) return true;
    if (size > rhs.uint64s().size()) return false;
    for (std::size_t i{size - 1}; i != static_cast<std::size_t>(-1); --i) {
        std::uint64_t const l{lhs.uint64s().data()[i]};
        std::uint64_t const r{rhs.uint64s().data()[i]};
        if (l < r) return true;
        if (l > r) return false;
    }
    return false;
}

inline bool operator>(cube_type const & lhs, cube_type const & rhs) {
    return operator<(rhs, lhs);
}

inline bool operator<=(cube_type const & lhs, cube_type const & rhs) {
    return not operator>(lhs, rhs);
}

inline bool operator>=(cube_type const & lhs, cube_type const & rhs) {
    return not operator<(lhs, rhs);
}


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_CUBE_TYPE_HPP
