#include "query_egt_test.hpp"


#include "probing.hpp"


TEST(QueryEgt, Kc6Re7Ng6kb4btmNormalFen) {
    std::string_view fen{"8/4R3/2K3N1/8/1k6/8/8/8 b - - 0 1"};
    std::optional<tb_position_type> tb_position_opt{parse_fen(fen)};
    ASSERT_TRUE(tb_position_opt.has_value());
    probe_report_type const probe_report{query_egt_directly(*tb_position_opt)};
    EXPECT_EQ(loss, probe_report);
}

TEST(QueryEgt, Kc6Re7Ng6kb4btmVisualFen) {
    std::string_view fen{"11111111/1111R111/11K111N1/11111111/"
     "1k111111/11111111/11111111/11111111 b - - 0 1"};
    std::optional<tb_position_type> tb_position_opt{parse_fen(fen)};
    ASSERT_TRUE(tb_position_opt.has_value());
    probe_report_type const probe_report{query_egt_directly(*tb_position_opt)};
    EXPECT_EQ(loss, probe_report);
}

// Pawn on b8 means it's really on b5 with the last move being b7-b5.
TEST(QueryEgt, Ka8Pa5Pa2kb2pb8wtmNormalFen) {
    std::string_view fen{"K7/8/8/Pp6/8/8/Pk6/8 w - b6 0 1"};
    std::optional<tb_position_type> tb_position_opt{parse_fen(fen)};
    ASSERT_TRUE(tb_position_opt.has_value());
    probe_report_type const probe_report{query_egt_directly(*tb_position_opt)};
    EXPECT_EQ(win, probe_report);
}
