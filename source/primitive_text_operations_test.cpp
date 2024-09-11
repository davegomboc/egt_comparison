#include "primitive_text_operations_test.hpp"


#include <vector>
#include <cstdint>


TEST(PrimitiveTextOperations, NoWhitespacePrefixToTrim) {
    std::string_view const expected("hello   ");
    std::string_view view("hello   ");
    trim_whitespace_prefix(view);
    EXPECT_EQ(expected, view);
}


TEST(PrimitiveTextOperations, WhitespacePrefixToTrim) {
    std::string_view const expected("hello   ");
    std::string_view view("\t  hello   ");
    trim_whitespace_prefix(view);
    EXPECT_EQ(expected, view);
}


TEST(PrimitiveTextOperations, NoWhitespaceSuffixToTrim) {
    std::string_view const expected("   hello");
    std::string_view view("   hello");
    trim_whitespace_suffix(view);
    EXPECT_EQ(expected, view);
}


TEST(PrimitiveTextOperations, WhitespaceSuffixToTrim) {
    std::string_view const expected("\t\thello");
    std::string_view view("\t\thello\t\t");
    trim_whitespace_suffix(view);
    EXPECT_EQ(expected, view);
}


TEST(PrimitiveTextOperations, WhitespaceToTrim) {
    std::string_view const expected("hello world");
    std::string_view view("\n\t hello world \t\n");
    trim_whitespace(view);
    EXPECT_EQ(expected, view);
}


TEST(PrimitiveTextOperations, NotFoundForLeftmostSplit) {
    std::string_view to_split("hello world: party: time");
    std::optional<std::pair<std::string_view, std::string_view>>
     split_result_opt{leftmost_split(to_split, '4')};
    ASSERT_EQ(std::nullopt, split_result_opt);
}


TEST(PrimitiveTextOperations, FirstCharCaseForLeftmostSplit) {
    std::string_view to_split("\tafter");
    std::string_view const expected_prefix("");
    std::string_view const expected_suffix("after");
    std::optional<std::pair<std::string_view, std::string_view>>
     split_result_opt{leftmost_split(to_split, '\t')};
    ASSERT_NE(std::nullopt, split_result_opt);
    EXPECT_EQ(expected_prefix, split_result_opt->first);
    EXPECT_EQ(expected_suffix, split_result_opt->second);
}


TEST(PrimitiveTextOperations, LastCharCaseForLeftmostSplit) {
    std::string_view to_split("before\n");
    std::string_view const expected_prefix("before");
    std::string_view const expected_suffix("");
    std::optional<std::pair<std::string_view, std::string_view>>
     split_result_opt{leftmost_split(to_split, '\n')};
    ASSERT_NE(std::nullopt, split_result_opt);
    EXPECT_EQ(expected_prefix, split_result_opt->first);
    EXPECT_EQ(expected_suffix, split_result_opt->second);
}


TEST(PrimitiveTextOperations, TypicalCaseForLeftmostSplit) {
    std::string_view to_split("2024-06-09T03:57:30-0800");
    std::string_view const expected_prefix("2024-06-09T03");
    std::string_view const expected_suffix("57:30-0800");
    std::optional<std::pair<std::string_view, std::string_view>>
     split_result_opt{leftmost_split(to_split, ':')};
    ASSERT_NE(std::nullopt, split_result_opt);
    EXPECT_EQ(expected_prefix, split_result_opt->first);
    EXPECT_EQ(expected_suffix, split_result_opt->second);
}
