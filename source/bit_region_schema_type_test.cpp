#include "bit_region_schema_type_test.hpp"


TEST(BitRegionSchemaType, GenerateSchemaForKingVsKing) {
    std::unique_ptr<bit_region_schema_type> const schema_ptr{
     bit_region_schema_type::create_ptr(13, std::vector<std::size_t>{}, 3)};
    ASSERT_NE(nullptr, schema_ptr);
    bit_region_schema_type const & schema{*schema_ptr};

    EXPECT_EQ(0, schema.first_var(var_kind_enum::binary));
    EXPECT_EQ(13, schema.past_last_var(var_kind_enum::binary));
    EXPECT_EQ(13, schema.first_var(var_kind_enum::multiple_valued));
    EXPECT_EQ(13, schema.past_last_var(var_kind_enum::multiple_valued));
    EXPECT_EQ(13, schema.first_var(var_kind_enum::output));
    EXPECT_EQ(14, schema.past_last_var(var_kind_enum::output));
    EXPECT_EQ(14, schema.var_count());

    EXPECT_EQ(0, schema.first_bit(0));
    EXPECT_EQ(2, schema.past_last_bit(0));
    EXPECT_EQ(2, schema.first_bit(1));
    EXPECT_EQ(4, schema.past_last_bit(1));
    EXPECT_EQ(4, schema.first_bit(2));
    EXPECT_EQ(6, schema.past_last_bit(2));
    EXPECT_EQ(6, schema.first_bit(3));
    EXPECT_EQ(8, schema.past_last_bit(3));
    EXPECT_EQ(8, schema.first_bit(4));
    EXPECT_EQ(10, schema.past_last_bit(4));
    EXPECT_EQ(10, schema.first_bit(5));
    EXPECT_EQ(12, schema.past_last_bit(5));
    EXPECT_EQ(12, schema.first_bit(6));
    EXPECT_EQ(14, schema.past_last_bit(6));
    EXPECT_EQ(14, schema.first_bit(7));
    EXPECT_EQ(16, schema.past_last_bit(7));
    EXPECT_EQ(16, schema.first_bit(8));
    EXPECT_EQ(18, schema.past_last_bit(8));
    EXPECT_EQ(18, schema.first_bit(9));
    EXPECT_EQ(20, schema.past_last_bit(9));
    EXPECT_EQ(20, schema.first_bit(10));
    EXPECT_EQ(22, schema.past_last_bit(10));
    EXPECT_EQ(22, schema.first_bit(11));
    EXPECT_EQ(24, schema.past_last_bit(11));
    EXPECT_EQ(24, schema.first_bit(12));
    EXPECT_EQ(26, schema.past_last_bit(12));
    EXPECT_EQ(26, schema.first_bit(13));
    EXPECT_EQ(29, schema.past_last_bit(13));
    EXPECT_EQ(29, schema.bit_count());
}


TEST(BitRegionSchemaType, GenerateSchemaContainingWidthTwoMultipleValuedVar) {
    std::unique_ptr<bit_region_schema_type> const schema_ptr{
     bit_region_schema_type::create_ptr(3, std::vector<std::size_t>{2, 3}, 1)};
    ASSERT_NE(nullptr, schema_ptr);
    bit_region_schema_type const & schema{*schema_ptr};

    EXPECT_EQ(0, schema.first_var(var_kind_enum::binary));
    EXPECT_EQ(3, schema.past_last_var(var_kind_enum::binary));
    EXPECT_EQ(3, schema.first_var(var_kind_enum::multiple_valued));
    EXPECT_EQ(5, schema.past_last_var(var_kind_enum::multiple_valued));
    EXPECT_EQ(5, schema.first_var(var_kind_enum::output));
    EXPECT_EQ(6, schema.past_last_var(var_kind_enum::output));
    EXPECT_EQ(6, schema.var_count());

    EXPECT_EQ(0, schema.first_bit(0));
    EXPECT_EQ(2, schema.past_last_bit(0));
    EXPECT_EQ(2, schema.first_bit(1));
    EXPECT_EQ(4, schema.past_last_bit(1));
    EXPECT_EQ(4, schema.first_bit(2));
    EXPECT_EQ(6, schema.past_last_bit(2));
    EXPECT_EQ(6, schema.first_bit(3));
    EXPECT_EQ(8, schema.past_last_bit(3));
    EXPECT_EQ(8, schema.first_bit(4));
    EXPECT_EQ(11, schema.past_last_bit(4));
    EXPECT_EQ(11, schema.first_bit(5));
    EXPECT_EQ(12, schema.past_last_bit(5));
    EXPECT_EQ(12, schema.bit_count());
}
