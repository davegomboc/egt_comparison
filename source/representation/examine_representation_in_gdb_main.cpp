#include <iostream>

#include "representation/coordinate_enum.hpp"
#include "representation/coordinate_type.hpp"
#include "representation/coordinates_type.hpp"
#include "representation/egt_position_type.hpp"
#include "representation/figurine_enum.hpp"
#include "representation/figurine_type.hpp"
#include "representation/figurines_type.hpp"
#include "representation/figurine_colour_enum.hpp"
#include "representation/figurine_colour_type.hpp"
#include "representation/figurine_placement_type.hpp"
#include "representation/figurine_placements_type.hpp"
#include "representation/figurine_shape_enum.hpp"
#include "representation/figurine_shape_type.hpp"
#include "representation/figurine_shapes_type.hpp"
#include "representation/logic_digit_type.hpp"
#include "representation/logic_digits_type.hpp"
#include "representation/material_side_type.hpp"
#include "representation/material_signature_type.hpp"



using namespace representation;


int main(void) {
    logic_digits_type logic_digits{sng, off, on, dc, dc, sng, on, off};
    logic_digits_type logic_digits_2{sng, off, on, dc, on, sng, sng, sng, on, on, on, off, off, dc, dc};
    std::cout << logic_digits << std::endl;
    std::cout << logic_digits_2 << std::endl;

    coordinates_type coordinates{a8, b7, c6, d5, h4, g3, f2, e1};
    std::cout << coordinates << std::endl;

    figurine_colour_enum fcew{figurine_colour_enum::white};
    figurine_colour_enum fceb{figurine_colour_enum::black};
    std::cout << fcew << fceb << std::endl;

    figurine_colour_type fctw{fcew};
    figurine_colour_type fctb{fceb};
    std::cout << fctw << fctb << std::endl;

    figurine_shape_enum fse_k{figurine_shape_enum::king};
    figurine_shape_enum fse_p{figurine_shape_enum::pawn};
    std::cout << fse_k << fse_p << std::endl;

    figurine_shape_type fst_k{king};
    figurine_shape_type fst_r{rook};
    std::cout << fst_k << fst_r << std::endl;

    figurine_shapes_type shapes{bishop, knight, pawn};
    std::cout << shapes << std::endl;

    figurine_enum fe_wk{figurine_enum::white_king};
    figurine_enum fe_bp{figurine_enum::black_pawn};
    std::cout << fe_wk << fe_bp << std::endl;

    figurine_type ft_wk{white_king};
    figurine_type ft_bb{black_bishop};
    std::cout << ft_wk << ft_bb << std::endl;

    figurines_type figurines{white_king, black_knight, black_pawn, white_pawn};
    std::cout << figurines << std::endl;

    std::optional<material_side_type> white_side_opt{
     material_side_type::create_opt(figurine_shapes_type{king, knight})};
    std::optional<material_side_type> black_side_opt{
     material_side_type::create_opt(figurine_shapes_type{king, pawn})};
    material_side_type white_side{*white_side_opt};
    material_side_type black_side{*black_side_opt};
    std::cout << white_side << black_side << std::endl;

    material_signature_type signature{
     std::move(white_side), std::move(black_side)};
    std::cout << signature << std::endl;

    figurine_placement_type wk{Kd2};
    figurine_placement_type wr{Rd1};
    figurine_placement_type bk{kf7};
    figurine_placement_type bn{ne6};
    std::cout << wk << wr << bk << bn << std::endl;

    figurine_placements_type placements{bn, bk, wk, wr};
    std::cout << placements << std::endl;

    egt_position_type pos(black, placements, no_ep);
    std::cout << pos << std::endl;

    std::cout << "Done." << std::endl;
}
