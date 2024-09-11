#include "epd/castling_rights_type_test.hpp"


using namespace epd;


TEST(CastlingRightsType, ParseEmptyString) {
    std::string const castling_rights{""};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_FALSE(castling_rights_opt);
}


TEST(CastlingRightsType, ParseAllCastlingRights){
    std::string const castling_rights{"KQkq"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_TRUE(castling_rights_opt);
    ASSERT_TRUE(castling_rights_opt->black_kingside());
    ASSERT_TRUE(castling_rights_opt->black_queenside());
    ASSERT_TRUE(castling_rights_opt->white_kingside());
    ASSERT_TRUE(castling_rights_opt->white_queenside());
    ASSERT_EQ(castling_rights, castling_rights_opt->as_epd_fen());
}


TEST(CastlingRightsType, ParseInvalidCastlingRights){
    std::string const castling_rights{"kqx"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_FALSE(castling_rights_opt);
}


TEST(CastlingRightsType, ParseNoCastlingRights) {
    std::string const castling_rights{"-"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_TRUE(castling_rights_opt);
    ASSERT_FALSE(castling_rights_opt->black_kingside());
    ASSERT_FALSE(castling_rights_opt->black_queenside());
    ASSERT_FALSE(castling_rights_opt->white_kingside());
    ASSERT_FALSE(castling_rights_opt->white_queenside());
    ASSERT_EQ(castling_rights, castling_rights_opt->as_epd_fen());
}


TEST(CastlingRightsType, ParseBlackKingsideOnly) {
    std::string const castling_rights{"k"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_TRUE(castling_rights_opt);
    ASSERT_TRUE(castling_rights_opt->black_kingside());
    ASSERT_FALSE(castling_rights_opt->black_queenside());
    ASSERT_FALSE(castling_rights_opt->white_kingside());
    ASSERT_FALSE(castling_rights_opt->white_queenside());
    ASSERT_EQ(castling_rights, castling_rights_opt->as_epd_fen());
}


TEST(CastlingRightsType, ParseBlackQueensideOnly) {
    std::string const castling_rights{"q"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_TRUE(castling_rights_opt);
    ASSERT_FALSE(castling_rights_opt->black_kingside());
    ASSERT_TRUE(castling_rights_opt->black_queenside());
    ASSERT_FALSE(castling_rights_opt->white_kingside());
    ASSERT_FALSE(castling_rights_opt->white_queenside());
    ASSERT_EQ(castling_rights, castling_rights_opt->as_epd_fen());
}


TEST(CastlingRightsType, ParseWhiteKingsideOnly) {
    std::string const castling_rights{"K"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_TRUE(castling_rights_opt);
    ASSERT_FALSE(castling_rights_opt->black_kingside());
    ASSERT_FALSE(castling_rights_opt->black_queenside());
    ASSERT_TRUE(castling_rights_opt->white_kingside());
    ASSERT_FALSE(castling_rights_opt->white_queenside());
    ASSERT_EQ(castling_rights, castling_rights_opt->as_epd_fen());
}


TEST(CastlingRightsType, ParseWhiteQueensideOnly) {
    std::string const castling_rights{"Q"};
    auto const castling_rights_opt{
     castling_rights_type::create(castling_rights)};
    ASSERT_TRUE(castling_rights_opt);
    ASSERT_FALSE(castling_rights_opt->black_kingside());
    ASSERT_FALSE(castling_rights_opt->black_queenside());
    ASSERT_FALSE(castling_rights_opt->white_kingside());
    ASSERT_TRUE(castling_rights_opt->white_queenside());
    ASSERT_EQ(castling_rights, castling_rights_opt->as_epd_fen());
}
