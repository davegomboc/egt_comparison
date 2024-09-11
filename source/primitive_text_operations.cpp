#include "primitive_text_operations.hpp"


#include <cctype>


bool contains(std::string_view const text, char const ch) {
    return std::string_view::npos != text.find(ch);
}


bool starts_with(
    std::string const & potential_elongation,
    std::string_view const & potential_prefix
) {
    return 0 == potential_elongation.rfind(potential_prefix, 0);
}


bool starts_with(
    std::string_view const & potential_elongation,
    std::string_view const & potential_prefix
) {
    return 0 == potential_elongation.rfind(potential_prefix, 0);
}


void trim_whitespace_prefix(std::string_view & text) {
    while (not text.empty()) {
        if (not std::isspace(text.front())) {
            break;
        }
        text.remove_prefix(1);
    }
}


void trim_whitespace_suffix(std::string_view & text) {
    while (not text.empty()) {
        if (not std::isspace(text.back())) {
            break;
        }
        text.remove_suffix(1);
    }
}


void trim_whitespace(std::string_view & text) {
    trim_whitespace_prefix(text);
    trim_whitespace_suffix(text);
}


std::optional<std::pair<std::string_view, std::string_view>> leftmost_split(
    std::string_view text,
    char ch
) {
    std::string_view::size_type split_position{text.find(ch, 0)};
    if (std::string_view::npos == split_position) {
        return std::nullopt;
    }
    if (0 == split_position) {
        text.remove_prefix(1);
        return std::make_pair<std::string_view, std::string_view>(
         "", std::move(text));
    }
    if (text.size() == split_position) {
        text.remove_suffix(1);
        return std::make_pair<std::string_view, std::string_view>(
         std::move(text), "");
    }
    std::string_view prefix(text);
    prefix.remove_suffix(text.size() - split_position);
    std::string_view suffix(text);
    suffix.remove_prefix(split_position + 1);
    return std::make_pair<std::string_view, std::string_view>(
     std::move(prefix), std::move(suffix));
}
