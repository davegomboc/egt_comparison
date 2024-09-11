#if !defined(EGT_COMPARISON_EPD_EN_PASSANT_TARGET_TYPE_HPP)
#define EGT_COMPARISON_EPD_EN_PASSANT_TARGET_TYPE_HPP


#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>
#include <optional>


namespace epd {


class en_passant_target_type {
public:
    en_passant_target_type & operator=(
     en_passant_target_type const &) = default;
    en_passant_target_type & operator=(en_passant_target_type &&) = default;
    constexpr en_passant_target_type(en_passant_target_type const &) = default;
    constexpr en_passant_target_type(en_passant_target_type &&) = default;
    ~en_passant_target_type() = default;

    constexpr en_passant_target_type() : en_passant_target_type(UINT8_C(0)) {}

    static std::optional<en_passant_target_type> create(char file, char rank);
    static std::optional<en_passant_target_type> create(
        std::string_view const en_passant_target);

    constexpr bool specified() const { return 0 != the_en_passant_target; }
    constexpr char file_as_char() const {
        return 'a' + (0b000111 & the_en_passant_target);
    }
    constexpr char rank_as_char() const {
        return '8' - ((0b111000 & the_en_passant_target) >> 3);
    }
    constexpr std::uint8_t as_uint8_t() const { return the_en_passant_target; }
    std::string as_epd_fen() const;

    void clear();
    void double_pawn_push(std::uint8_t const file, bool const played_by_black);
    void double_pawn_push(char const file, bool const played_by_black);

private:
    constexpr en_passant_target_type(unsigned char const en_passant_target)
      : the_en_passant_target(en_passant_target) {}

    // 0 = none, 16..23 = a6..h6, 40..47 = a3..h3
    std::uint8_t the_en_passant_target;
};


std::ostream & operator<<(
    std::ostream & out,
    en_passant_target_type const & en_passant_target);


} // namespace epd


#endif // EGT_COMPARISON_EPD_EN_PASSANT_TARGET_TYPE_HPP
