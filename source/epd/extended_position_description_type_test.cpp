#include "epd/extended_position_description_type_test.hpp"


using namespace epd;


TEST(ExtendedPositionDescriptionType, ParseEPDRecordsWithGTV) {
    std::array<std::string, 30> const test_epd_strings{
        "8/3K4/4R3/8/5k2/8/8/8 b - - GTV l; fmvn 1; hmvc 0;",
        "8/1P6/8/8/8/8/2K5/6k1 b - - GTV l; fmvn 1; hmvc 0;",
        "8/7k/8/8/2K1N3/8/8/8 w - - GTV d; fmvn 1; hmvc 0;",
        "8/Q7/8/8/1k6/6K1/8/8 b - - GTV l; fmvn 1; hmvc 0;",
        "4K3/2k5/7P/8/8/8/8/8 w - - GTV w; fmvn 1; hmvc 0;",
        "8/4N3/6K1/8/8/8/1k6/8 b - - GTV d; fmvn 1; hmvc 0;",
        "2K5/8/8/1k6/8/8/6R1/8 w - - GTV w; fmvn 1; hmvc 0;",
        "8/8/3K2R1/8/8/8/8/3k4 b - - GTV l; fmvn 1; hmvc 0;",
        "8/8/8/8/N2K3k/8/8/8 w - - GTV d; fmvn 1; hmvc 0;",
        "8/8/8/8/2Q5/4K3/8/1k6 b - - GTV l; fmvn 1; hmvc 0;",
        "8/8/8/2K5/8/1kR5/1n6/8 b - - GTV d; fmvn 1; hmvc 0;",
        "8/pP6/8/8/8/3k4/7K/8 b - - GTV l; fmvn 1; hmvc 0;",
        "8/8/8/1K6/8/2k5/6P1/6Q1 w - - GTV w; fmvn 1; hmvc 0;",
        "8/5P2/3k4/8/8/6K1/7N/8 w - - GTV w; fmvn 1; hmvc 0;",
        "7n/8/2R5/8/8/8/8/k4K2 w - - GTV w; fmvn 1; hmvc 0;",
        "5b2/8/4K3/8/8/5k2/8/6R1 w - - GTV w; fmvn 1; hmvc 0;",
        "8/6R1/8/7p/8/8/6k1/2K5 b - - GTV l; fmvn 1; hmvc 0;",
        "1K6/8/4R3/1k6/8/7Q/8/8 b - - GTV l; fmvn 1; hmvc 0;",
        "8/8/5K2/8/8/1R1k4/8/3N4 b - - GTV l; fmvn 1; hmvc 0;",
        "8/5K2/2k5/8/8/2N5/4p3/8 w - - GTV d; fmvn 1; hmvc 0;",
        "8/8/2P5/2k5/4B3/8/6N1/3K4 w - - GTV w; fmvn 1; hmvc 0;",
        "7r/8/2N5/8/1K6/6k1/8/5Q2 w - - GTV w; fmvn 1; hmvc 0;",
        "8/8/6K1/8/2n5/3N1k2/8/6N1 b - - GTV d; fmvn 1; hmvc 0;",
        "8/8/2B4k/3K2p1/4B3/8/8/8 w - - GTV d; fmvn 1; hmvc 0;",
        "4K3/8/1R6/b7/4k3/8/8/2B5 b - - GTV d; fmvn 1; hmvc 0;",
        "8/K7/8/8/P5P1/2k4P/8/8 b - - GTV l; fmvn 1; hmvc 0;",
        "8/8/2rR4/4K3/8/8/N7/4k3 b - - GTV d; fmvn 1; hmvc 0;",
        "5N1Q/8/8/6Q1/1K6/8/8/2k5 b - - GTV l; fmvn 1; hmvc 0;",
        "8/k7/4P3/7B/8/8/3R4/6K1 w - - GTV w; fmvn 1; hmvc 0;",
        "8/3P4/2K5/n7/6k1/8/8/4Q3 w - - GTV w; fmvn 1; hmvc 0;",
    };
    for (std::size_t i{}; i != test_epd_strings.size(); ++i) {
        auto const epd_opt{extended_position_description_type::create(
         test_epd_strings[i])};
        ASSERT_TRUE(epd_opt);
        ASSERT_EQ(test_epd_strings[i], epd_opt->as_epd());
    }
}


TEST(ExtendedPositionDescriptionType, ParseFENRecords) {
    std::array<std::string, 30> const test_fen_strings{
        "8/3K4/4R3/8/5k2/8/8/8 b - - 0 1",
        "8/1P6/8/8/8/8/2K5/6k1 b - - 0 1",
        "8/7k/8/8/2K1N3/8/8/8 w - - 0 1",
        "8/Q7/8/8/1k6/6K1/8/8 b - - 0 1",
        "4K3/2k5/7P/8/8/8/8/8 w - - 0 1",
        "8/4N3/6K1/8/8/8/1k6/8 b - - 0 1",
        "2K5/8/8/1k6/8/8/6R1/8 w - - 0 1",
        "8/8/3K2R1/8/8/8/8/3k4 b - - 0 1",
        "8/8/8/8/N2K3k/8/8/8 w - - 0 1",
        "8/8/8/8/2Q5/4K3/8/1k6 b - - 0 1",
        "8/8/8/2K5/8/1kR5/1n6/8 b - - 0 1",
        "8/pP6/8/8/8/3k4/7K/8 b - - 0 1",
        "8/8/8/1K6/8/2k5/6P1/6Q1 w - - 0 1",
        "8/5P2/3k4/8/8/6K1/7N/8 w - - 0 1",
        "7n/8/2R5/8/8/8/8/k4K2 w - - 0 1",
        "5b2/8/4K3/8/8/5k2/8/6R1 w - - 0 1",
        "8/6R1/8/7p/8/8/6k1/2K5 b - - 0 1",
        "1K6/8/4R3/1k6/8/7Q/8/8 b - - 0 1",
        "8/8/5K2/8/8/1R1k4/8/3N4 b - - 0 1",
        "8/5K2/2k5/8/8/2N5/4p3/8 w - - 0 1",
        "8/8/2P5/2k5/4B3/8/6N1/3K4 w - - 0 1",
        "7r/8/2N5/8/1K6/6k1/8/5Q2 w - - 0 1",
        "8/8/6K1/8/2n5/3N1k2/8/6N1 b - - 0 1",
        "8/8/2B4k/3K2p1/4B3/8/8/8 w - - 0 1",
        "4K3/8/1R6/b7/4k3/8/8/2B5 b - - 0 1",
        "8/K7/8/8/P5P1/2k4P/8/8 b - - 0 1",
        "8/8/2rR4/4K3/8/8/N7/4k3 b - - 0 1",
        "5N1Q/8/8/6Q1/1K6/8/8/2k5 b - - 0 1",
        "8/k7/4P3/7B/8/8/3R4/6K1 w - - 0 1",
        "8/3P4/2K5/n7/6k1/8/8/4Q3 w - - 0 1",
    };
    for (std::size_t i{}; i != test_fen_strings.size(); ++i) {
        auto const epd_opt{extended_position_description_type::create_from_fen(
         test_fen_strings[i])};
        ASSERT_TRUE(epd_opt);
        std::optional<std::string> fen_text_opt{epd_opt->as_fen_opt()};
        ASSERT_TRUE(fen_text_opt);
        ASSERT_EQ(test_fen_strings[i], *fen_text_opt);
    }
}


TEST(ExtendedPositionDescriptionType, AsFenOptWithoutOperations) {
    std::string test_epd_string{"8/3K4/4R3/8/5k2/8/8/8 b - -"};
    auto const epd_opt{extended_position_description_type::create(
     test_epd_string)};
    ASSERT_TRUE(epd_opt);
    ASSERT_FALSE(epd_opt->as_fen_opt());
}


TEST(ExtendedPositionDescriptionType, AsFenOptWhenFmvnMissing) {
    std::string test_epd_string{"8/3K4/4R3/8/5k2/8/8/8 b - - GTV l; hmvc 0;"};
    auto const epd_opt{extended_position_description_type::create(
     test_epd_string)};
    ASSERT_TRUE(epd_opt);
    ASSERT_FALSE(epd_opt->as_fen_opt());
}


TEST(ExtendedPositionDescriptionType, AsFenOptWhenHmvcMissing) {
    std::string test_epd_string{"8/3K4/4R3/8/5k2/8/8/8 b - - GTV l; fmvn 1;"};
    auto const epd_opt{extended_position_description_type::create(
     test_epd_string)};
    ASSERT_TRUE(epd_opt);
    ASSERT_FALSE(epd_opt->as_fen_opt());
}


TEST(ExtendedPositionDescroptionType, AsFenOptWhenFmvnAndHmvcPresent) {
    std::string test_epd_string{
     "8/3K4/4R3/8/5k2/8/8/8 b - - GTV l; fmvn 1; hmvc 0;"};
    auto const epd_opt{extended_position_description_type::create(
     test_epd_string)};
    ASSERT_TRUE(epd_opt);
    std::optional<std::string> fen_opt{epd_opt->as_fen_opt()};
    ASSERT_TRUE(fen_opt);
    ASSERT_EQ("8/3K4/4R3/8/5k2/8/8/8 b - - 0 1", *fen_opt);
}
