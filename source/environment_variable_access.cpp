#include "environment_variable_access.hpp"


#include <cassert>


std::optional<std::string>
get_environment_variable_opt(char const * const name_ptr) {
    if (nullptr == name_ptr) {
        return std::nullopt;
    }
    char * value_ptr{std::getenv(name_ptr)};
    if (nullptr == value_ptr) {
        return std::nullopt;
    }
    return std::string{value_ptr};
}

std::optional<std::string>
get_environment_variable_opt(std::string_view const name) {
    return get_environment_variable_opt(std::string{name}.c_str());
}

std::optional<std::filesystem::path> get_home_directory_opt() {
    bool const on_windows{std::nullopt != get_environment_variable_opt("WINDIR")};
    std::optional<std::string> home_drive_opt;
    std::optional<std::string> home_path_opt;
    if (on_windows) {
        home_drive_opt = get_environment_variable_opt("HOMEDRIVE");
        home_path_opt = get_environment_variable_opt("HOMEPATH");
    } else {
        home_drive_opt = std::string{};
        home_path_opt = get_environment_variable_opt("HOME");
    }
    if ((std::nullopt == home_path_opt) or (std::nullopt == home_drive_opt)) {
        return std::nullopt;
    }
    return std::filesystem::path{*home_drive_opt + *home_path_opt};
}

std::optional<std::filesystem::path> get_scratch_directory_opt() {
    std::optional<std::string> scratch_opt{get_environment_variable_opt("SCRATCH")};
    if (scratch_opt) {
        return std::filesystem::path(*scratch_opt);
    }
    return get_temporary_directory_opt();
}

std::optional<std::filesystem::path> get_temporary_directory_opt() {
    std::error_code error_code;
    std::filesystem::path temporary_directory{
     std::filesystem::temp_directory_path(error_code)};
    if ("" == temporary_directory) {
        return std::nullopt;
    }
    return temporary_directory;
}


std::map<std::string, std::string> environment_variables() {
    extern char ** environ;
    std::map<std::string, std::string> result;
    for (char ** current{environ}; nullptr != current; ++current) {
        if (nullptr == *current) {
            break;
        }
        std::string_view const setting{*current};
        std::size_t const key_value_separator_location{setting.find('=')};
        assert(std::string_view::npos != key_value_separator_location);
        std::string const key{setting.substr(0, key_value_separator_location)};
        std::string const value{
         setting.substr(1 + key_value_separator_location)};
        assert(result.end() == result.find(key));
        result[key] = value;
    }
    return result;
}
