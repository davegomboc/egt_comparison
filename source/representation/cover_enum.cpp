#include "representation/cover_enum.hpp"


#include <cassert>
#include <cstddef>
#include <ostream>


namespace representation {


std::string_view as_string_view(cover_enum cover) {
    assert(cover_enum_is_valid(cover));
    return cover_enum_names[static_cast<unsigned char>(cover)];
}


std::ostream & operator<<(std::ostream & out, cover_enum kind) {
    return out << as_string_view(kind);
}


std::optional<cover_enum> as_cover_enum(std::string_view name) {
    for (std::size_t nth_cover{}; nth_cover != cover_enum_names.size();
     ++nth_cover) {
        if (name == cover_enum_names[nth_cover]) {
            return static_cast<cover_enum>(nth_cover);
        }
    }
    return std::nullopt;
}


} // namespace representation
