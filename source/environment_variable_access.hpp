#if !defined(EGT_COMPARISON_ENVIRONMENT_VARIABLE_ACCESS_HPP)
#define EGT_COMPARISON_ENVIRONMENT_VARIABLE_ACCESS_HPP


#include <cstdlib>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>


std::map<std::string, std::string> environment_variables();

std::optional<std::string> get_environment_variable_opt(
    char const * const name_ptr);
std::optional<std::string> get_environment_variable_opt(
    std::string_view const name);

std::optional<std::filesystem::path> get_home_directory_opt();

std::optional<std::filesystem::path> get_scratch_directory_opt();

std::optional<std::filesystem::path> get_temporary_directory_opt();


#endif // EGT_COMPARISON_ENVIRONMENT_VARIABLE_ACCESS_HPP
