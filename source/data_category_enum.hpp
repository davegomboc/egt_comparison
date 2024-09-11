#ifndef EGT_COMPARISON_DATA_CATEGORY_ENUM_HPP
#define EGT_COMPARISON_DATA_CATEGORY_ENUM_HPP


#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <optional>


enum class data_category_enum : unsigned char {
    chesspresso_binary = 0,
    cube_list = 1,
    ddag = 2,
    indexed_cube_list = 3,
};


inline std::filesystem::path extension(data_category_enum const category) {
    switch (category) {
    case data_category_enum::chesspresso_binary:
        return "plb";
    case data_category_enum::cube_list:
        return "clst";
    case data_category_enum::ddag:
        return "dtr";
    case data_category_enum::indexed_cube_list:
        return "icl";
    default: /* nothing */;
    }
    std::cerr << "EE: Bad data_category_enum "
     << static_cast<unsigned int>(category) << ".\n";
    std::exit(EXIT_FAILURE);
}


inline std::optional<data_category_enum>
categorize_by_extension_opt(std::filesystem::path const & path) {
    std::filesystem::path const path_extension(path.extension());
    if (".clst" == path_extension) {
        return data_category_enum::cube_list;
    } else if (".dtr" == path_extension) {
        return data_category_enum::ddag;
    } else if (".icl" == path_extension) {
        return data_category_enum::indexed_cube_list;
     } else if (".plb" == path_extension) {
        return data_category_enum::chesspresso_binary;
    }
    return std::nullopt;
}


#endif // EGT_COMPARISON_DATA_CATEGORY_ENUM_HPP
