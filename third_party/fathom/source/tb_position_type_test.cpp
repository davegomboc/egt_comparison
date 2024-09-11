#include "tb_position_type_test.hpp"


#include "tbprobe.h"


/*
   Syzygy tb_position_type bitboards are mapped as follows:

   bit 63 62 61 60 59 58 57 56 55 54 53 ... 10 09 08 07 06 05 04 03 02 01 00
square h8 g8 f8 e8 d8 c8 b8 a8 h7 g7 f7 ... c2 b2 a2 h1 g1 f1 e1 d1 c1 b1 a1
*/

constexpr std::string_view empty_board{
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111 w - - 0 1"};

constexpr std::string_view ka8{
 "k1111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111 w - - 0 1"};

constexpr std::string_view kh8{
 "1111111k/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111 w - - 0 1"};

constexpr std::string_view Ka1{
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "K1111111 w - - 0 1"};

constexpr std::string_view Kh1{
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "11111111/"
 "1111111K w - - 0 1"};

constexpr std::string_view fen_after_1_e4_c5{
 "rnbqkbnr/"
 "pp1ppppp/"
 "11111111/"
 "11p11111/"
 "1111P111/"
 "11111111/"
 "PPPP1PPP/"
 "RNBQKBNR w KQkq c6 0 2"
};

constexpr std::string_view fen_after_1_e4_c5_2_Nf3{
 "rnbqkbnr/"
 "pp1ppppp/"
 "11111111/"
 "11p11111/"
 "1111P111/"
 "11111N11/"
 "PPPP1PPP/"
 "RNBQKB1R b KQkq - 1 2"
};

constexpr tb_position_type garbage_tb_position{
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint64_t>(-1),
 static_cast<std::uint8_t>(-1),
 static_cast<std::uint8_t>(-1),
 static_cast<std::uint8_t>(-1),
 static_cast<bool>(-1),
 static_cast<std::uint16_t>(-1)};

TEST(ParseFen, Empty) {
    std::optional<tb_position_type> tb_position_opt{parse_fen("")};
    EXPECT_FALSE(tb_position_opt);
}

TEST(SegmentFen, EmptyBoard) {
    std::optional<std::vector<std::string_view>> segments_opt{
     segment_fen(empty_board)};
    EXPECT_TRUE(segments_opt);
    std::vector<std::string_view> const & segments{*segments_opt};
    EXPECT_EQ(6, segments.size());
    EXPECT_EQ("11111111/11111111/11111111/11111111/11111111/11111111/11111111/11111111", segments[0]);
    EXPECT_EQ("w", segments[1]);
    EXPECT_EQ("-", segments[2]);
    EXPECT_EQ("-", segments[3]);
    EXPECT_EQ("0", segments[4]);
    EXPECT_EQ("1", segments[5]);
}

TEST(ParseFenBoard, EmptyBoard) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_board(tb_position, "8/11111111/11111111/11111111/11111111/11111111/11111111/11111111")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(0, tb_position.white);
    EXPECT_EQ(0, tb_position.black);
    EXPECT_EQ(0, tb_position.kings);
    EXPECT_EQ(0, tb_position.queens);
    EXPECT_EQ(0, tb_position.rooks);
    EXPECT_EQ(0, tb_position.bishops);
    EXPECT_EQ(0, tb_position.knights);
    EXPECT_EQ(0, tb_position.pawns);
}

TEST(ParseFenBoard, Qh8) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_board(tb_position, "1111111Q/11111111/11111111/11111111/11111111/11111111/11111111/11111111")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(1ull << 63, tb_position.white);
    EXPECT_EQ(0, tb_position.black);
    EXPECT_EQ(0, tb_position.kings);
    EXPECT_EQ(1ull << 63, tb_position.queens);
    EXPECT_EQ(0, tb_position.rooks);
    EXPECT_EQ(0, tb_position.bishops);
    EXPECT_EQ(0, tb_position.knights);
    EXPECT_EQ(0, tb_position.pawns);
}

TEST(ParseFenBoard, ra8) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_board(tb_position, "r1111111/11111111/11111111/11111111/11111111/11111111/11111111/11111111")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(0, tb_position.white);
    EXPECT_EQ(1ull << 56, tb_position.black);
    EXPECT_EQ(0, tb_position.kings);
    EXPECT_EQ(0, tb_position.queens);
    EXPECT_EQ(1ull << 56, tb_position.rooks);
    EXPECT_EQ(0, tb_position.bishops);
    EXPECT_EQ(0, tb_position.knights);
    EXPECT_EQ(0, tb_position.pawns);
}

TEST(ParseFenBoard, bh1) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_board(tb_position, "8/11111111/11111111/11111111/11111111/11111111/11111111/1111111b")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(0, tb_position.white);
    EXPECT_EQ(1ull << 7, tb_position.black);
    EXPECT_EQ(0, tb_position.kings);
    EXPECT_EQ(0, tb_position.queens);
    EXPECT_EQ(0, tb_position.rooks);
    EXPECT_EQ(1ull << 7, tb_position.bishops);
    EXPECT_EQ(0, tb_position.knights);
    EXPECT_EQ(0, tb_position.pawns);
}

TEST(ParseFenBoard, Na1) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_board(tb_position, "8/11111111/11111111/11111111/11111111/11111111/11111111/N1111111")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(1ull << 0, tb_position.white);
    EXPECT_EQ(0, tb_position.black);
    EXPECT_EQ(0, tb_position.kings);
    EXPECT_EQ(0, tb_position.queens);
    EXPECT_EQ(0, tb_position.rooks);
    EXPECT_EQ(0, tb_position.bishops);
    EXPECT_EQ(1ull << 0, tb_position.knights);
    EXPECT_EQ(0, tb_position.pawns);
}

TEST(ParseFenBoard, StartBoard) {
    tb_position_type tb_position{garbage_tb_position};
    bool parse_result{parse_fen_board(tb_position,
     "rnbqkbnr/pppppppp/11111111/11111111/11111111/11111111/PPPPPPPP/RNBQKBNR")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'11111111ull,
     tb_position.white);
    EXPECT_EQ(
     0b11111111'11111111'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position.black);
    EXPECT_EQ(
     0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00010000ull,
     tb_position.kings);
    EXPECT_EQ(
     0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00001000ull,
     tb_position.queens);
    EXPECT_EQ(
     0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'10000001ull,
     tb_position.rooks);
    EXPECT_EQ(
     0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00100100ull,
     tb_position.bishops);
    EXPECT_EQ(
     0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'01000010ull,
     tb_position.knights);
    EXPECT_EQ(
     0b00000000'11111111'00000000'00000000'00000000'00000000'11111111'00000000ull,
     tb_position.pawns);
}

TEST(ParseFenSideToMove, Invalid) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_side_to_move(tb_position, "-")};
    EXPECT_FALSE(parse_result);
}

TEST(ParseFenSideToMove, TooLong) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_side_to_move(tb_position, "wb")};
    EXPECT_FALSE(parse_result);
}

TEST(ParseFenSideToMove, Black) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_side_to_move(tb_position, "b")};
    EXPECT_TRUE(parse_result);
    EXPECT_FALSE(tb_position.white_to_move);
}

TEST(ParseFenSideToMove, White) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_side_to_move(tb_position, "w")};
    EXPECT_TRUE(parse_result);
    EXPECT_TRUE(tb_position.white_to_move);
}

TEST(ParseFenCastlingRights, All) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_castling_rights(tb_position, "KQkq")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(TB_CASTLING_K | TB_CASTLING_Q | TB_CASTLING_k | TB_CASTLING_q,
     tb_position.castling);
}

TEST(ParseFenCastlingRights, BlackKingsideWhiteQueenside) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_castling_rights(tb_position, "Qk")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(TB_CASTLING_Q | TB_CASTLING_k, tb_position.castling);
}

TEST(ParseFenCastlingRights, Invalid) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_castling_rights(tb_position, "x")};
    EXPECT_FALSE(parse_result);
}

TEST(ParseFenCastlingRights, KingsideOnly) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_castling_rights(tb_position, "Kk")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(TB_CASTLING_K | TB_CASTLING_k, tb_position.castling);
}

TEST(ParseFenCastlingRights, None) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_castling_rights(tb_position, "-")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(0, tb_position.castling);
}

TEST(ParseFenCastlingRights, QueensideOnly) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_castling_rights(tb_position, "Qq")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(TB_CASTLING_Q | TB_CASTLING_q, tb_position.castling);
}

TEST(ParseFenEnPassantRights, Invalid) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_en_passant_rights(tb_position, "x0")};
    EXPECT_FALSE(parse_result);
}

TEST(ParseFenEnPassantRights, None) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_en_passant_rights(tb_position, "-")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(0, tb_position.en_passant);
}

TEST(ParseFenEnPassantRights, a3) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_en_passant_rights(tb_position, "a3")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(16, tb_position.en_passant);
}

TEST(ParseFenEnPassantRights, h6) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{parse_fen_en_passant_rights(tb_position, "h6")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(47, tb_position.en_passant);
}

TEST(ParseFenHalfMovesSinceProgressCount, Invalid) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_half_moves_since_progress_count(tb_position, "x")};
    EXPECT_FALSE(parse_result);
}

TEST(ParseFenHalfMovesSinceProgressCount, Zero) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_half_moves_since_progress_count(tb_position, "0")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(0, tb_position.rule50);
}

TEST(ParseFenHalfMovesSinceProgressCount, TwentyFive) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_half_moves_since_progress_count(tb_position, "25")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(25, tb_position.rule50);
}

TEST(ParseFenHalfMovesSinceProgressCount, NinetyNine) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_half_moves_since_progress_count(tb_position, "99")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(99, tb_position.rule50);
}

TEST(ParseFenHalfMovesSinceProgressCount, OneHundred) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_half_moves_since_progress_count(tb_position, "100")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(100, tb_position.rule50);
}

TEST(ParseFenHalfMovesSinceProgressCount, OneHundredOne) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_half_moves_since_progress_count(tb_position, "101")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(101, tb_position.rule50);
}

TEST(ParseFenFullMoveCountCount, Zero) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_full_move_count(tb_position, "0")};
    EXPECT_FALSE(parse_result);
}

TEST(ParseFenFullMoveCountCount, One) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_full_move_count(tb_position, "1")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(1, tb_position.full_move_count);
}

TEST(ParseFenFullMoveCountCount, SevenHundredFifty) {
    tb_position_type tb_position{garbage_tb_position};
    bool const parse_result{
     parse_fen_full_move_count(tb_position, "750")};
    EXPECT_TRUE(parse_result);
    EXPECT_EQ(750, tb_position.full_move_count);
}

TEST(ParseFen, EmptyBoard) {
    std::optional<tb_position_type> tb_position_opt{parse_fen(empty_board)};
    EXPECT_TRUE(tb_position_opt);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->black);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->white);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->kings);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->queens);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->rooks);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->bishops);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->knights);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->pawns);
    EXPECT_EQ(0, tb_position_opt->castling);
    EXPECT_EQ(0, tb_position_opt->rule50);
    EXPECT_EQ(0, tb_position_opt->en_passant);
    EXPECT_TRUE(tb_position_opt->white_to_move);
    EXPECT_EQ(1, tb_position_opt->full_move_count);
}

TEST(ParseFen, kh8) {
    std::optional<tb_position_type> tb_position_opt{parse_fen(kh8)};
    EXPECT_TRUE(tb_position_opt);
    EXPECT_EQ(
     0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->black);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->white);
    EXPECT_EQ(
     0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->kings);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->queens);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->rooks);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->bishops);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->knights);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->pawns);
    EXPECT_EQ(0, tb_position_opt->castling);
    EXPECT_EQ(0, tb_position_opt->rule50);
    EXPECT_EQ(0, tb_position_opt->en_passant);
    EXPECT_TRUE(tb_position_opt->white_to_move);
    EXPECT_EQ(1, tb_position_opt->full_move_count);
}

TEST(ParseFen, ka8) {
    std::optional<tb_position_type> tb_position_opt{parse_fen(ka8)};
    EXPECT_TRUE(tb_position_opt);
    EXPECT_EQ(
     0b00000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->black);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->white);
    EXPECT_EQ(
     0b00000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->kings);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->queens);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->rooks);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->bishops);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->knights);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->pawns);
    EXPECT_EQ(0, tb_position_opt->castling);
    EXPECT_EQ(0, tb_position_opt->rule50);
    EXPECT_EQ(0, tb_position_opt->en_passant);
    EXPECT_TRUE(tb_position_opt->white_to_move);
    EXPECT_EQ(1, tb_position_opt->full_move_count);
}

TEST(ParseFen, Kh1) {
    std::optional<tb_position_type> tb_position_opt{parse_fen(Kh1)};
    EXPECT_TRUE(tb_position_opt);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->black);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000000ull,
     tb_position_opt->white);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000000ull,
     tb_position_opt->kings);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->queens);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->rooks);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->bishops);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->knights);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->pawns);
    EXPECT_EQ(0, tb_position_opt->castling);
    EXPECT_EQ(0, tb_position_opt->rule50);
    EXPECT_EQ(0, tb_position_opt->en_passant);
    EXPECT_TRUE(tb_position_opt->white_to_move);
    EXPECT_EQ(1, tb_position_opt->full_move_count);
}

TEST(ParseFen, Ka1) {
    std::optional<tb_position_type> tb_position_opt{parse_fen(Ka1)};
    EXPECT_TRUE(tb_position_opt);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->black);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001ull,
     tb_position_opt->white);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001ull,
     tb_position_opt->kings);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->queens);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->rooks);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->bishops);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->knights);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000ull,
     tb_position_opt->pawns);
    EXPECT_EQ(0, tb_position_opt->castling);
    EXPECT_EQ(0, tb_position_opt->rule50);
    EXPECT_EQ(0, tb_position_opt->en_passant);
    EXPECT_TRUE(tb_position_opt->white_to_move);
    EXPECT_EQ(1, tb_position_opt->full_move_count);
}

TEST(Parse, After_1_e4_c5) {
    std::optional<tb_position_type> const tb_position_opt{
     parse_fen(fen_after_1_e4_c5)};
    EXPECT_TRUE(tb_position_opt);
    EXPECT_EQ(
     0b11111111'11111011'00000000'00000100'00000000'00000000'00000000'00000000ull,
     tb_position_opt->black);
    EXPECT_EQ(
     0b00000000'00000000'00000000'00000000'00010000'00000000'11101111'11111111ull,
     tb_position_opt->white);
    EXPECT_EQ(
     0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00010000ull,
     tb_position_opt->kings);
    EXPECT_EQ(
     0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00001000ull,
     tb_position_opt->queens);
    EXPECT_EQ(
     0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'10000001ull,
     tb_position_opt->rooks);
    EXPECT_EQ(
     0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00100100ull,
     tb_position_opt->bishops);
    EXPECT_EQ(
     0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'01000010ull,
     tb_position_opt->knights);
    EXPECT_EQ(
     0b00000000'11111011'00000000'00000100'00010000'00000000'11101111'00000000ull,
     tb_position_opt->pawns);
    EXPECT_EQ(TB_CASTLING_K | TB_CASTLING_Q | TB_CASTLING_k | TB_CASTLING_q,
     tb_position_opt->castling);
    EXPECT_EQ(42, tb_position_opt->en_passant);
    EXPECT_EQ(0, tb_position_opt->rule50);
    EXPECT_EQ(2, tb_position_opt->full_move_count);
}

TEST(FenBoardSegment, After_1_e4_c5) {
    std::optional<tb_position_type> const tb_position_opt{
     parse_fen(fen_after_1_e4_c5)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen_board_segment{
     tb_position_opt->fen_board_segment(true)};
    EXPECT_EQ(
     "rnbqkbnr/pp1ppppp/11111111/11p11111/1111P111/11111111/PPPP1PPP/RNBQKBNR",
     fen_board_segment);
}

TEST(FenSideToMoveSegment, Black) {
    tb_position_type tb_position;
    tb_position.white_to_move = false;
    std::string const fen_side_to_move_segment{
     tb_position.fen_side_to_move_segment()};
    EXPECT_EQ("b", fen_side_to_move_segment);
}

TEST(FenSideToMoveSegment, White) {
    tb_position_type tb_position;
    tb_position.white_to_move = true;
    std::string const fen_side_to_move_segment{
     tb_position.fen_side_to_move_segment()};
    EXPECT_EQ("w", fen_side_to_move_segment);
}

TEST(FenCastlingRightsSegment, All) {
    tb_position_type tb_position;
    tb_position.castling =
     TB_CASTLING_K | TB_CASTLING_Q | TB_CASTLING_k | TB_CASTLING_q;
    std::string const fen_castling_rights_segment{
     tb_position.fen_castling_rights_segment()};
    EXPECT_EQ("KQkq", fen_castling_rights_segment);
}

TEST(FenCastlingRightsSegment, WhiteKingsideOnly) {
    tb_position_type tb_position;
    tb_position.castling = TB_CASTLING_K;
    std::string const fen_castling_rights_segment{
     tb_position.fen_castling_rights_segment()};
    EXPECT_EQ("K", fen_castling_rights_segment);
}

TEST(FenCastlingRightsSegment, WhiteQueensideOnly) {
    tb_position_type tb_position;
    tb_position.castling = TB_CASTLING_Q;
    std::string const fen_castling_rights_segment{
     tb_position.fen_castling_rights_segment()};
    EXPECT_EQ("Q", fen_castling_rights_segment);
}

TEST(FenCastlingRightsSegment, BlackKingsideOnly) {
    tb_position_type tb_position;
    tb_position.castling = TB_CASTLING_k;
    std::string const fen_castling_rights_segment{
     tb_position.fen_castling_rights_segment()};
    EXPECT_EQ("k", fen_castling_rights_segment);
}

TEST(FenCastlingRightsSegment, BlackQueensideOnly) {
    tb_position_type tb_position;
    tb_position.castling = TB_CASTLING_q;
    std::string const fen_castling_rights_segment{
     tb_position.fen_castling_rights_segment()};
    EXPECT_EQ("q", fen_castling_rights_segment);
}

TEST(FenCastlingRightsSegment, None) {
    tb_position_type tb_position;
    tb_position.castling = 0;
    std::string const fen_castling_rights_segment{
     tb_position.fen_castling_rights_segment()};
    EXPECT_EQ("-", fen_castling_rights_segment);
}

TEST(FenEnPassantRightsSegment, None) {
    tb_position_type tb_position;
    tb_position.en_passant = 0;
    std::string const fen_en_passant_rights_segment{
     tb_position.fen_en_passant_rights_segment()};
    EXPECT_EQ("-", fen_en_passant_rights_segment);
}

TEST(FenEnPassantRightsSegment, e3) {
    tb_position_type tb_position;
    tb_position.en_passant = 20;
    std::string const fen_en_passant_rights_segment{
     tb_position.fen_en_passant_rights_segment()};
    EXPECT_EQ("e3", fen_en_passant_rights_segment);
}

TEST(FenEnPassantRightsSegment, a6) {
    tb_position_type tb_position;
    tb_position.en_passant = 40;
    std::string const fen_en_passant_rights_segment{
     tb_position.fen_en_passant_rights_segment()};
    EXPECT_EQ("a6", fen_en_passant_rights_segment);
}

TEST(RoundTrip, ka8) {
    std::optional<tb_position_type> const tb_position_opt{parse_fen(ka8)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen{tb_position_opt->as_fen(true)};
    EXPECT_EQ(fen, ka8);
}

TEST(RoundTrip, kh8) {
    std::optional<tb_position_type> const tb_position_opt{parse_fen(kh8)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen{tb_position_opt->as_fen(true)};
    EXPECT_EQ(fen, kh8);
}

TEST(RoundTrip, Ka1) {
    std::optional<tb_position_type> const tb_position_opt{parse_fen(Ka1)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen{tb_position_opt->as_fen(true)};
    EXPECT_EQ(fen, Ka1);
}

TEST(RoundTrip, Kh1) {
    std::optional<tb_position_type> const tb_position_opt{parse_fen(Kh1)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen{tb_position_opt->as_fen(true)};
    EXPECT_EQ(fen, Kh1);
}

TEST(RoundTrip, FenAftere4c5) {
    std::optional<tb_position_type> const tb_position_opt{
     parse_fen(fen_after_1_e4_c5)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen{tb_position_opt->as_fen(true)};
    EXPECT_EQ(fen, fen_after_1_e4_c5);
}

TEST(RoundTrip, FenAftere4c5Nf3) {
    std::optional<tb_position_type> const tb_position_opt{
     parse_fen(fen_after_1_e4_c5_2_Nf3)};
    EXPECT_TRUE(tb_position_opt);
    std::string const fen{tb_position_opt->as_fen(true)};
    EXPECT_EQ(fen, fen_after_1_e4_c5_2_Nf3);
}
