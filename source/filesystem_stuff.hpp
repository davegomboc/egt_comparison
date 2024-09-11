#ifndef EGT_COMPARISON_FILESYSTEM_STUFF_HPP
#define EGT_COMPARISON_FILESYSTEM_STUFF_HPP


#include <cassert>
#include <filesystem>
#include <optional>
#include <system_error>

#include "data_category_enum.hpp"


inline bool good(std::istream & data_in) {
    if (data_in.good()) {
        return true;
    }
    std::cerr << "EE: data_in.good() is false.\nTT: data_in.bad() = "
     << data_in.bad() << "\nTT: data_in.eof() = " << data_in.eof()
     << "\nTT: data_in.fail() = " << data_in.fail() << '\n';
    return false;
}

// Returns true iff an error occurred.
inline bool update_permissions(
    std::filesystem::path const & path,
    std::filesystem::perms const permissions,
    std::filesystem::perm_options const permission_options
) {
    std::error_code error_code;
    std::filesystem::permissions(path, permissions, permission_options,
     error_code);
    if (std::error_code() == error_code) {
        return false;
    }
    std::cerr << "WW: Error " << error_code
     << " when attempting to set permissions on " << path << ".\n";
    return true;
}

// Returns true iff an error occurred.
inline bool rename_path(
    std::filesystem::path const from,
    std::filesystem::path const to
) {
    std::error_code const no_error;
    std::error_code error_code;
    std::cerr << "II: Renaming from " << from << " to " << to << ".\n";
    std::filesystem::rename(from, to, error_code);
    if (no_error == error_code) {
        return false;
    }
    std::cerr << "WW: Rename failed: error_code = " << error_code << ".\n";
    return true;
}

// Returns true iff an error occurred.
inline bool create_directories_except_filename(std::filesystem::path path) {
    if (path.has_filename()) {
        path.remove_filename();
    }
    std::error_code error_code;
    std::filesystem::create_directories(path, error_code);
    if (std::error_code() == error_code) {
        return false;
    }
    std::cerr << "EE: Error " << error_code
     << " when attempting to create directories " << path << ".\n";
    return true;
}

inline std::optional<std::filesystem::path>
canonical_path_opt(std::filesystem::path const & path) {
    std::error_code error_code;
    std::filesystem::path const canonical_path(
     std::filesystem::canonical(path, error_code));
    if (std::error_code() == error_code) {
        return canonical_path;
    }
    return std::nullopt;
}

inline std::filesystem::path data_file_path(
    std::filesystem::path const & preexisting_directory_path,
    data_category_enum const category,
    std::string_view const material_balance,
    std::string_view const proc
) {
    std::filesystem::path result(preexisting_directory_path);
    result /= material_balance;
    if (not proc.empty()) {
        result += '_';
        result += proc;
    }
    result += '.';
    result += extension(category);
    return result;
}

// Attempts to find where (both indexed and non-indexed) cube lists could be
// stored on disk.  Returns std::nullopt when it doesn't have a good guess.
inline std::optional<std::filesystem::path>
cube_list_directory_path_opt(std::string_view const material_balance) {
    std::error_code error_code;
    char const * const egt_path_as_c_str{std::getenv("EGT_DIR")};
    if (egt_path_as_c_str) {
        std::filesystem::path candidate_directory_path(egt_path_as_c_str);
        candidate_directory_path /= "cube_lists";
        candidate_directory_path /= std::to_string(
         material_balance.size() - 4 /* _vs_ */);
        if (std::filesystem::exists(candidate_directory_path, error_code)) {
            return candidate_directory_path;
        }
    }
    return std::nullopt;
}

// Attempts to find where the (either indexed or non-indexed) cube list file
// either is or ought to be located on disk.  Returns std::nullopt when it
// doesn't have a good guess.
inline std::optional<std::filesystem::path> cube_list_path_opt(
    std::string_view const material_balance,
    std::string_view const proc,
    bool const indexed
) {
    std::optional<std::filesystem::path> directory_path_opt{
     cube_list_directory_path_opt(material_balance)};
    if (std::nullopt == directory_path_opt) {
        return std::nullopt;
    }
    std::filesystem::path result{*directory_path_opt};
    result /= material_balance;
    if (not proc.empty()) {
        result += '_';
        result += proc;
    }
    result += '.';
    if (indexed) {
        result += extension(data_category_enum::indexed_cube_list);
    } else {
        result += extension(data_category_enum::cube_list);
    }
    return result;
}

inline std::optional<std::filesystem::path> cube_list_path_opt(
    std::string_view const material_balance,
    unsigned int const distance,
    std::string_view const proc_suffix,
    bool const indexed
) {
    std::string proc;
    if (0 < distance) {
        proc = "D";
        if (distance < 10) {
            proc += '0';
        }
        proc += std::to_string(distance);
        proc += 'I';
        if (not proc_suffix.empty()) {
            proc += '_';
        }
    }
    if (not proc_suffix.empty()) {
        proc += proc_suffix;
    }
    return cube_list_path_opt(material_balance, proc, indexed);
}


#endif // EGT_COMPARISON_FILESYSTEM_STUFF_HPP
