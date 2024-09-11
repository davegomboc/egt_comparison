#if !defined(EGT_COMPARISON_REPRESENTATION_EN_PASSANT_INFORMATION_TYPE)
#define EGT_COMPARISON_REPRESENTATION_EN_PASSANT_INFORMATION_TYPE


#include "representation/coordinate_enum.hpp"
#include "representation/coordinate_type.hpp"
#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_type.hpp"


namespace representation {


class en_passant_information_type {
public:
    en_passant_information_type & operator=(
     en_passant_information_type const &) = default;
    en_passant_information_type & operator=(
     en_passant_information_type &&) = default;
    en_passant_information_type(en_passant_information_type const &) = default;
    en_passant_information_type(en_passant_information_type &&) = default;
    ~en_passant_information_type() = default;

    static std::optional<en_passant_information_type> create_opt(
     coordinate_type const en_passant_target_square);

    figurine_colour_type colour_of_pawn_just_pushed_two_squares() const {
        return rank_enum::rank_3 == the_en_passant_target_rank ? white : black;
    }

    figurine_type target_figurine() const {
        return figurine_type(colour_of_pawn_just_pushed_two_squares(), pawn);
    }

    coordinate_type actual_coordinate() const {
        return coordinate_type(the_en_passant_target_file, actual_rank());
    }

    coordinate_type en_passant_target_coordinate() const {
        return coordinate_type(the_en_passant_target_file,
         the_en_passant_target_rank);
    }

    coordinate_type illegal_encoded_coordinate() const {
        rank_enum const encoded_rank(rank_enum::rank_3
         == the_en_passant_target_rank ? rank_enum::rank_1 : rank_enum::rank_8);
        return coordinate_type(the_en_passant_target_file, encoded_rank);
    }

    std::vector<coordinate_type> en_passant_capture_initiation_coordinates() {
        std::vector<coordinate_type> result;
        if (file_enum::a_file != the_en_passant_target_file) {
            file_enum prev_file(the_en_passant_target_file);
            --prev_file;
            result.emplace_back(prev_file, actual_rank());
        }
        if (file_enum::h_file != the_en_passant_target_file) {
            file_enum next_file(the_en_passant_target_file);
            ++next_file;
            result.emplace_back(next_file, actual_rank());
        }
        return result;
    }

private:
    en_passant_information_type(coordinate_type const en_passant_target);

    rank_enum actual_rank() const {
        return (rank_enum::rank_3 == the_en_passant_target_rank)
         ? rank_enum::rank_4 : rank_enum::rank_5;
    }

    // The file of the pawn that just moved two squares.
    file_enum the_en_passant_target_file;

    // The rank where an opposing pawn would end up after using the en passant
    // rule to capture the pawn that just moved two squares.
    rank_enum the_en_passant_target_rank;
};


} // namespace representation


#endif // EGT_COMPARISON_REPRESENTATION_EN_PASSANT_INFORMATION_TYPE
