#if !defined(EGT_COMPARISON_PRIMITIVE_TEXT_OPERATIONS_HPP)
#define EGT_COMPARISON_PRIMITIVE_TEXT_OPERATIONS_HPP


#include <optional>
#include <string>
#include <string_view>
#include <utility>


bool contains(std::string_view const text, char ch);

bool starts_with(
    std::string_view const & potential_elongation,
    std::string_view const & potential_prefix);

bool starts_with(
    std::string const & potential_elongation,
    std::string_view const & potential_prefix);


void trim_whitespace_prefix(std::string_view & text);

void trim_whitespace_suffix(std::string_view & text);

void trim_whitespace(std::string_view & text);


std::optional<std::pair<std::string_view, std::string_view>> leftmost_split(
    std::string_view text,
    char ch);


#endif // EGT_COMPARISON_PRIMITIVE_TEXT_OPERATIONS_HPP
