#include "representation/figurine_enum_test.hpp"


#include <iomanip>
#include <iostream>
#include <sstream>


using representation::figurine_enum;
using representation::figurine_enum_opt;
using representation::logic_digits_type;
using representation::off;
using representation::on;


TEST(FigurineEnum, Validity) {
    EXPECT_FALSE(valid(static_cast<figurine_enum>(0)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(1)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(2)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(3)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(4)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(5)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(6)));
    EXPECT_FALSE(valid(static_cast<figurine_enum>(7)));
    EXPECT_FALSE(valid(static_cast<figurine_enum>(8)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(9)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(10)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(11)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(12)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(13)));
    EXPECT_TRUE(valid(static_cast<figurine_enum>(14)));
    EXPECT_FALSE(valid(static_cast<figurine_enum>(15)));
    EXPECT_FALSE(valid(static_cast<figurine_enum>(16)));
    EXPECT_FALSE(valid(static_cast<figurine_enum>(17)));
}


TEST(FigurineEnum, AsLogicDigits) {
    EXPECT_EQ((logic_digits_type{ off, off, off,  on}),
     as_logic_digits(figurine_enum::white_king));
    EXPECT_EQ((logic_digits_type{ off, off,  on, off}),
     as_logic_digits(figurine_enum::white_queen));
    EXPECT_EQ((logic_digits_type{ off, off,  on,  on}),
     as_logic_digits(figurine_enum::white_rook));
    EXPECT_EQ((logic_digits_type{ off,  on, off, off}),
     as_logic_digits(figurine_enum::white_bishop));
    EXPECT_EQ((logic_digits_type{ off,  on, off,  on}),
     as_logic_digits(figurine_enum::white_knight));
    EXPECT_EQ((logic_digits_type{ off,  on,  on, off}),
     as_logic_digits(figurine_enum::white_pawn));
    EXPECT_EQ((logic_digits_type{  on, off, off,  on}),
     as_logic_digits(figurine_enum::black_king));
    EXPECT_EQ((logic_digits_type{  on, off,  on, off}),
     as_logic_digits(figurine_enum::black_queen));
    EXPECT_EQ((logic_digits_type{  on, off,  on,  on}),
     as_logic_digits(figurine_enum::black_rook));
    EXPECT_EQ((logic_digits_type{  on,  on, off, off}),
     as_logic_digits(figurine_enum::black_bishop));
    EXPECT_EQ((logic_digits_type{  on,  on, off,  on}),
     as_logic_digits(figurine_enum::black_knight));
    EXPECT_EQ((logic_digits_type{  on,  on,  on, off}),
     as_logic_digits(figurine_enum::black_pawn));
}


TEST(FigurineEnum, Inserter) {
    std::ostringstream out;
    out << figurine_enum::white_pawn << figurine_enum::black_king
     << figurine_enum::white_knight << figurine_enum::black_queen
     << figurine_enum::white_bishop << figurine_enum::black_rook
     << figurine_enum::white_rook << figurine_enum::black_bishop
     << figurine_enum::white_queen << figurine_enum::black_knight
     << figurine_enum::white_king << figurine_enum::black_pawn;
    EXPECT_EQ("PkNqBrRbQnKp", out.str());
}


TEST(FigurineEnum, FigurineEnumOpt) {
    std::optional<figurine_enum> white_king_opt{figurine_enum_opt('K')};
    std::optional<figurine_enum> white_queen_opt{figurine_enum_opt('Q')};
    std::optional<figurine_enum> white_rook_opt{figurine_enum_opt('R')};
    std::optional<figurine_enum> white_bishop_opt{figurine_enum_opt('B')};
    std::optional<figurine_enum> white_knight_opt{figurine_enum_opt('N')};
    std::optional<figurine_enum> white_pawn_opt{figurine_enum_opt('P')};
    std::optional<figurine_enum> black_king_opt{figurine_enum_opt('k')};
    std::optional<figurine_enum> black_queen_opt{figurine_enum_opt('q')};
    std::optional<figurine_enum> black_rook_opt{figurine_enum_opt('r')};
    std::optional<figurine_enum> black_bishop_opt{figurine_enum_opt('b')};
    std::optional<figurine_enum> black_knight_opt{figurine_enum_opt('n')};
    std::optional<figurine_enum> black_pawn_opt{figurine_enum_opt('p')};
    std::optional<figurine_enum> underscore_opt{figurine_enum_opt('_')};
    std::optional<figurine_enum> s_opt{figurine_enum_opt('s')};
    std::optional<figurine_enum> v_opt{figurine_enum_opt('v')};
    std::optional<figurine_enum> A_opt{figurine_enum_opt('A')};
    std::optional<figurine_enum> Z_opt{figurine_enum_opt('Z')};

    ASSERT_TRUE(white_king_opt);
    ASSERT_TRUE(white_queen_opt);
    ASSERT_TRUE(white_rook_opt);
    ASSERT_TRUE(white_bishop_opt);
    ASSERT_TRUE(white_knight_opt);
    ASSERT_TRUE(white_pawn_opt);
    ASSERT_TRUE(black_king_opt);
    ASSERT_TRUE(black_queen_opt);
    ASSERT_TRUE(black_rook_opt);
    ASSERT_TRUE(black_bishop_opt);
    ASSERT_TRUE(black_knight_opt);
    ASSERT_TRUE(black_pawn_opt);
    EXPECT_FALSE(underscore_opt);
    EXPECT_FALSE(s_opt);
    EXPECT_FALSE(v_opt);
    EXPECT_FALSE(A_opt);
    EXPECT_FALSE(Z_opt);

    EXPECT_EQ(*white_king_opt, figurine_enum::white_king);
    EXPECT_EQ(*white_queen_opt, figurine_enum::white_queen);
    EXPECT_EQ(*white_rook_opt, figurine_enum::white_rook);
    EXPECT_EQ(*white_bishop_opt, figurine_enum::white_bishop);
    EXPECT_EQ(*white_knight_opt, figurine_enum::white_knight);
    EXPECT_EQ(*white_pawn_opt, figurine_enum::white_pawn);
    EXPECT_EQ(*black_king_opt, figurine_enum::black_king);
    EXPECT_EQ(*black_queen_opt, figurine_enum::black_queen);
    EXPECT_EQ(*black_rook_opt, figurine_enum::black_rook);
    EXPECT_EQ(*black_bishop_opt, figurine_enum::black_bishop);
    EXPECT_EQ(*black_knight_opt, figurine_enum::black_knight);
    EXPECT_EQ(*black_pawn_opt, figurine_enum::black_pawn);
}


TEST(FigurineEnum, OutputToTerminal) {
    for (unsigned int i{}; i != 16; ++i) {
        figurine_enum const figurine{static_cast<figurine_enum>(i)};
        if (not valid(figurine)) continue;
        std::cout << std::setw(2) << i << " => "
         << representation::as_char(figurine) << ' '
         << representation::as_standalone_utf8(figurine) << ' '
         << representation::as_board_diagram_square(figurine, false, false)
         << ' '
         << representation::as_board_diagram_square(figurine,  true, false)
         << ' '
         << representation::as_board_diagram_square(figurine, false,  true)
         << ' '
         << representation::as_board_diagram_square(figurine,  true,  true)
         << '\n';
    }
}
