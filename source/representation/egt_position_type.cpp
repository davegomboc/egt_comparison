#include "representation/egt_position_type.hpp"


#include <cassert>
#include <ostream>
#include <sstream>


namespace representation {


egt_position_type::egt_position_type(
    figurine_colour_type const colour_to_move,
    figurine_placements_type const & placements,
    coordinate_type const en_passant_target,
    epd::castling_rights_type const castling_rights
) : the_colour_to_move(colour_to_move),
    the_placements(placements),
    the_en_passant_target(en_passant_target),
    the_castling_rights(castling_rights)
{
    // TODO: What validity checking is necessary for probing purposes?
}


egt_position_type::egt_position_type(
    epd::board_contents_type const & board_contents,
    epd::side_to_move_type const side_to_move,
    epd::castling_rights_type const castling_rights,
    epd::en_passant_target_type const en_passant_target
) : the_colour_to_move(side_to_move),
    the_placements(board_contents),
    the_en_passant_target(coordinate_type(en_passant_target)),
    the_castling_rights{castling_rights} {}


egt_position_type::egt_position_type(
    epd::extended_position_description_type const & epd
) : egt_position_type(epd.board_contents(), epd.side_to_move(),
     epd.castling_rights(), epd.en_passant_target()) {}


std::string egt_position_type::as_string() const {
    std::ostringstream out;
    out << the_placements.as_string();
    out.put(' ');
    out.put(the_colour_to_move.as_char());
    out.put(' ');
    out << the_castling_rights.as_epd_fen();
    out.put(' ');
    if (no_ep == the_en_passant_target) {
        out.put('-');
    } else {
        out << the_en_passant_target.as_string();
    }
    return out.str();
}


std::optional<tb_position_type> egt_position_type::as_tb_position_opt() const {
    auto const [overlapping, bitboards]{the_placements.as_bitboards()};
    if (overlapping) return std::nullopt;
    std::uint8_t tb_position_en_passant{};
    if (no_ep != the_en_passant_target) {
        file_enum const file{just_file(the_en_passant_target.as_enum())};
        rank_enum const egt_position_rank{
         just_rank(the_en_passant_target.as_enum())};
        tb_position_en_passant = static_cast<std::uint8_t>(
         static_cast<std::uint8_t>(rank_enum::rank_1)
         - static_cast<std::uint8_t>(egt_position_rank)
         + static_cast<std::uint8_t>(file));
    }
    return bitboards.as_tb_position_type(white == the_colour_to_move, 1,
     tb_position_en_passant, the_castling_rights.as_uint8_t(), 0);
}


std::optional<epd::extended_position_description_type>
egt_position_type::as_extended_position_description_opt() const {
    auto const [overlapping, board_contents]{placements().as_board_contents()};
    std::optional<epd::en_passant_target_type> en_passant_target_opt{
     epd::en_passant_target_type::create(en_passant_target().as_string())};
    if (overlapping or std::nullopt == en_passant_target_opt) {
        return std::nullopt;
    }
    epd::extended_position_description_type const result(board_contents,
     colour_to_move().as_side_to_move(), castling_rights(),
     *en_passant_target_opt, epd::operations_type());
    return result;
}


std::size_t egt_position_type::as_position_id() const {
    std::size_t result{black_to_move() ? UINT64_C(1) : UINT64_C(0)};
    std::optional<en_passant_information_type> en_passant_info_opt(
     en_passant_information_opt());
    if (en_passant_info_opt) {
        figurine_placement_type const remove(en_passant_info_opt->target_figurine(),
         en_passant_info_opt->actual_coordinate());
        figurine_placement_type const add(en_passant_info_opt->target_figurine(),
         en_passant_info_opt->illegal_encoded_coordinate());
        figurine_placements_type encoded_placements(the_placements);
        encoded_placements.replace(remove, add);
        encoded_placements.append(result);
    } else {
        the_placements.append(result);
    }
    // TODO: Address castling.
    return result;
}


std::vector<bool> egt_position_type::as_bool_vector_id() const {
    std::size_t const input_bit_pair_count(the_placements.input_bit_pair_count());
    std::size_t const position_id(as_position_id());
    return representation::as_bool_vector_id(input_bit_pair_count, position_id);
}


std::vector<unsigned char> egt_position_type::as_mtbdd_uchar_vector_id() const {
    std::size_t const input_bit_pair_count(the_placements.input_bit_pair_count());
    std::size_t const position_id(as_position_id());
    return representation::as_mtbdd_uchar_vector_id(input_bit_pair_count, position_id);
}


logic_digits_type egt_position_type::as_logic_digits() const {
    logic_digits_type result{logic_digit_type{black_to_move()}};
    result.append(encoded_placements().as_logic_digits());
    return result;
}


material_side_type egt_position_type::as_material_side(
    figurine_colour_type colour
) const {
    figurine_shapes_type shapes{placements().figurines().shapes(colour)};
    std::optional<material_side_type> side_opt{
     material_side_type::create_opt(shapes)};
    assert(side_opt);
    return *side_opt;
}


material_signature_type egt_position_type::as_material_signature() const {
    return material_signature_type(as_material_side(representation::white),
     as_material_side(representation::black));
}


void egt_position_type::flip_a8h1s() {
    // the_colour_to_move is intentionally unchanged.
    the_placements.flip_a8h1s();
    assert(the_castling_rights.none());
    if (no_ep != the_en_passant_target) the_en_passant_target.flip_a8h1();
}


void egt_position_type::flip_colours() {
    the_colour_to_move.flip();
    the_placements.flip_colours();
    the_castling_rights.flip_colours();
    // the_en_passant_target is intentionally unchanged.
}


void egt_position_type::flip_files() {
    // the_colour_to_move is intentionally unchanged.
    the_placements.flip_files();
    assert(the_castling_rights.none());
    if (no_ep != the_en_passant_target) the_en_passant_target.flip_file();
}


void egt_position_type::flip_ranks() {
    // the_colour_to_move is intentionally unchanged.
    the_placements.flip_ranks();
    assert(the_castling_rights.none());
    if (no_ep != the_en_passant_target) the_en_passant_target.flip_rank();
}


void egt_position_type::invert() {
    the_colour_to_move.flip();
    the_placements.invert();
    the_castling_rights.flip_colours();
    if (no_ep != the_en_passant_target) the_en_passant_target.flip_rank();
}


void egt_position_type::restore_ordering_invariant() {
    the_placements.restore_ordering_invariant();
}


std::vector<egt_position_type>
egt_position_type::equal_material_pawnless_symmetrical_positions() const {
    std::size_t const symmetry_count{32};
    std::vector<egt_position_type> result(symmetry_count, *this);
    for (std::size_t i{}; i != symmetry_count; ++i) {
        if (i & 0b00001) result[i].invert();
        if (i & 0b00010) result[i].flip_files();
        if (i & 0b00100) result[i].flip_ranks();
        if (i & 0b01000) result[i].flip_a8h1s();
        if (i & 0b10000) result[i].flip_colours();
        if (0 != i) result[i].restore_ordering_invariant();
    }
    return result;
}


std::vector<egt_position_type>
egt_position_type::nonequal_material_pawnless_symmetrical_positions() const {
    std::size_t const symmetry_count{16};
    std::vector<egt_position_type> result(symmetry_count, *this);
    for (std::size_t i{}; i != symmetry_count; ++i) {
        if (i & 0b0001) result[i].invert();
        if (i & 0b0010) result[i].flip_files();
        if (i & 0b0100) result[i].flip_ranks();
        if (i & 0b1000) result[i].flip_a8h1s();
        if (0 != i) result[i].restore_ordering_invariant();
    }
    return result;
}


std::vector<egt_position_type>
egt_position_type::pawnful_symmetrical_positions() const {
    std::size_t const symmetry_count{4};
    std::vector<egt_position_type> result(symmetry_count, *this);
    for (std::size_t i{}; i != symmetry_count; ++i) {
        if (i & 0b01) result[i].invert();
        if (i & 0b10) result[i].flip_files();
        if (0 != i) result[i].restore_ordering_invariant();
    }
    return result;
}


std::vector<egt_position_type>
egt_position_type::symmetrical_positions() const {
    material_signature_type const material_signature(as_material_signature());

    bool const pawnless{material_signature.pawnless()};
    if (not pawnless) {
        return pawnful_symmetrical_positions();
    }

    bool const equal{material_signature.equal()};
    if (not equal) {
        return nonequal_material_pawnless_symmetrical_positions();
    }

    return equal_material_pawnless_symmetrical_positions();
}


egt_position_type egt_position_type::canonical_position(
    canonicalization_enum const canonicalization_mode
) const {
    if (canonicalization_enum::all_symmetries == canonicalization_mode) {
        std::vector<egt_position_type> symmetries{symmetrical_positions()};
        return *std::min_element(symmetries.begin(), symmetries.end());
    }
    assert(canonicalization_enum::single_symmetry == canonicalization_mode);
    egt_position_type inverted(*this);
    inverted.invert();
    inverted.restore_ordering_invariant();
    assert(inverted != *this);
    return (inverted < *this) ? inverted : *this;
}


void egt_position_type::canonicalize(
    canonicalization_enum const canonicalization_mode
) {
    *this = canonical_position(canonicalization_mode);
}


coordinate_type egt_position_type::decode_illegal_pawn_placement() {
    //std::cerr << "DD: Entered egt_position_type::decode_illegal_pawn_"
    // "placement().\n";
    //std::cerr << "TT: egt_position = " << *this << '\n';
    if (no_ep != the_en_passant_target) {
        //std::cerr << "TT: The en passant target field is already set!\n";
        //std::cerr << "DD: Exiting egt_position_type"
        // "::decode_illegal_pawn_placement(), returning no_ep.\n";
        return no_ep;
    }
    if (not the_placements.figurines().en_passant_can_happen()) {
        //std::cerr << "TT: Both sides must have a pawn for en passant to be "
        // "possible.\n";
        //std::cerr << "DD: Exiting egt_position_type"
        // "::decode_illegal_pawn_placement(), returning no_ep.\n";
        return no_ep;
    }
    coordinate_type candidate_en_passant_target{no_ep};
    for (figurine_placement_type const placement: the_placements) {
        //std::cerr << "TT: placement = " << placement << '\n';
        coordinate_type const potential_en_passant_target{
         placement.identify_en_passant_target_from_illegal_pawn_placement()};
        //std::cerr << "TT: potential_en_passant_target = "
        // << potential_en_passant_target << std::endl;
        if (no_ep == potential_en_passant_target) {
            //std::cerr << "TT: no_ep, so continuing.\n";
            continue;
        }
        if (no_ep != candidate_en_passant_target) {
            // A second candidate has been found, which isn't actually possible,
            // so the position is bogus (either invalid or illegal).
            //std::cerr << "TT: At least two pawns are placed on their back rank, "
            // "so the position is bogus.\n";
            //std::cerr << "DD: Exiting egt_position_type"
            // "::decode_illegal_pawn_placement(), returning no_ep.\n";
            return no_ep;
        }
        candidate_en_passant_target = potential_en_passant_target;
        //std::cerr << "TT: candidate_en_passant_target = "
        // << candidate_en_passant_target << '\n';
    }
    if (no_ep == candidate_en_passant_target) {
        //std::cerr << "TT: No pawn is in place to make an en passant capture.\n";
        //std::cerr << "DD: Exiting egt_position_type"
        // "::decode_illegal_pawn_placement(), returning no_ep.\n";
        return no_ep;
    }
    //std::cerr << "TT: The single candidate en passant target is "
    // << candidate_en_passant_target << ".\n";

    // We want to filter out positions where the opposing side has no pawn in
    // place to capture the en passant target.
    file_enum const candidate_target_file{
     just_file(candidate_en_passant_target.as_enum())};
    rank_enum const candidate_target_rank{
     just_rank(candidate_en_passant_target.as_enum())};
    assert((rank_enum::rank_3 == candidate_target_rank)
     or (rank_enum::rank_6 == candidate_target_rank));
    figurine_type const capturing_figurine{
     rank_enum::rank_3 == candidate_target_rank ? black_pawn : white_pawn};

    // We want to filter out positions where the side to move matches the side
    // that has the illegally-placed pawn, because only the opposing player may
    // capture en passant.  So, this is a bogus position.
    if (capturing_figurine.colour() != the_colour_to_move) {
        //std::cerr << "TT: Colour to move mismatch detected, so the position is "
        // "bogus.\n";
        //std::cerr << "DD: Exiting egt_position_type"
        // "::decode_illegal_pawn_placement(), returning no_ep.\n";
        return no_ep;
    }

    file_enum file_of_candidate_one{candidate_target_file};
    if (file_enum::a_file == candidate_target_file) {
        ++file_of_candidate_one;
    } else {
        --file_of_candidate_one;
    }
    file_enum file_of_candidate_two{candidate_target_file};
    if (file_enum::h_file == candidate_target_file) {
        --file_of_candidate_two;
    } else {
        ++file_of_candidate_two;
    }
    rank_enum rank_of_candidates{candidate_target_rank};
    if (rank_enum::rank_3 == candidate_target_rank) {
        ++rank_of_candidates;
    } else {
        --rank_of_candidates;
    }

    figurine_placement_type const candidate_one(capturing_figurine,
     file_of_candidate_one | rank_of_candidates);
    figurine_placement_type const candidate_two(capturing_figurine,
     file_of_candidate_two | rank_of_candidates);

    if (not the_placements.contains(candidate_one)
     and not the_placements.contains(candidate_two)) {
        return no_ep;
    }

    // Note that we do not actually test the legality of the en passant pawn
    // capture(s).
    figurine_type const just_moved_figurine{
     rank_enum::rank_3 == candidate_target_rank ? white_pawn : black_pawn};
    rank_enum const rank_of_illegal_pawn_placement{rank_enum::rank_3
     == candidate_target_rank ? rank_enum::rank_1 : rank_enum::rank_8};
    coordinate_type const illegally_placed_pawn_coordinate(
     candidate_target_file | rank_of_illegal_pawn_placement);
    rank_enum const actual_pawn_rank{rank_enum::rank_3
     == candidate_target_rank ? rank_enum::rank_4 : rank_enum::rank_5};
    coordinate_type const actual_pawn_coordinate(
     candidate_target_file | actual_pawn_rank);
    figurine_placement_type const illegal_figurine_placement(
     just_moved_figurine, illegally_placed_pawn_coordinate);
    figurine_placement_type const correct_figurine_placement(
     just_moved_figurine, actual_pawn_coordinate);
    the_placements.replace(illegal_figurine_placement,
     correct_figurine_placement);
    the_en_passant_target = candidate_en_passant_target;
    return candidate_en_passant_target;
}


std::optional<en_passant_information_type>
egt_position_type::en_passant_information_opt() const {
    if (no_ep == the_en_passant_target) {
        return std::nullopt;
    }
    if (not the_placements.figurines().en_passant_can_happen()) {
        return std::nullopt;
    }
    std::optional<en_passant_information_type> en_passant_info_opt{
     en_passant_information_type::create_opt(the_en_passant_target)};
    if (std::nullopt == en_passant_info_opt) {
        return std::nullopt;
    }
    if (en_passant_info_opt->colour_of_pawn_just_pushed_two_squares()
     == the_colour_to_move) {
        return std::nullopt;
    }
    return en_passant_info_opt;
}


figurine_placements_type egt_position_type::encoded_placements() const {
    std::optional<en_passant_information_type> en_passant_info_opt(
     en_passant_information_opt());
    if (std::nullopt == en_passant_info_opt) {
        return the_placements;
    }
    figurine_placement_type const remove(en_passant_info_opt->target_figurine(),
     en_passant_info_opt->actual_coordinate());
    figurine_placement_type const add(en_passant_info_opt->target_figurine(),
     en_passant_info_opt->illegal_encoded_coordinate());
    figurine_placements_type result(the_placements);
    result.replace(remove, add);
    return result;
}


void egt_position_type::encode_illegal_pawn_placement() {
    figurine_placements_type encoded(encoded_placements());
    if (encoded != the_placements) {
        the_placements = encoded;
        the_en_passant_target = no_ep;
    }
}


bool operator==(egt_position_type const & lhs, egt_position_type const & rhs) {
    if (lhs.white_to_move() != rhs.white_to_move()) return false;
    if (lhs.placements() != rhs.placements()) return false;
    if (lhs.castling_rights() != rhs.castling_rights()) return false;
    // TODO: In operator<() and here, address that the PGN standard requires
    // that EPD and FEN indicate whenever a pawn moved two squares last move
    // (instead of just if an en passant capture is actually possible!), but
    // that two such positions reaching the same destination state are
    // nonetheless equivalent whenever no en passant capture is actually
    // possible in either of them.
    if (lhs.en_passant_target() != rhs.en_passant_target()) return false;
    return true;
}


bool operator!=(egt_position_type const & lhs, egt_position_type const & rhs) {
    return not (lhs == rhs);
}


bool operator<(egt_position_type const & lhs, egt_position_type const & rhs) {
    material_signature_type const lhs_signature(lhs.as_material_signature());
    material_signature_type const rhs_signature(rhs.as_material_signature());
    if (lhs_signature != rhs_signature) {
        return lhs_signature < rhs_signature;
    }
    bool const lhs_black_to_move{not lhs.white_to_move()};
    bool const rhs_black_to_move{not rhs.white_to_move()};
    if (lhs_black_to_move != rhs_black_to_move) {
        return lhs_black_to_move < rhs_black_to_move;
    }
    figurine_placements_type const lhs_placements{lhs.placements()};
    figurine_placements_type const rhs_placements{rhs.placements()};
    if (lhs_placements != rhs_placements) {
        return lhs_placements < rhs_placements;
    }
    epd::castling_rights_type const lhs_castling_rights{lhs.castling_rights()};
    epd::castling_rights_type const rhs_castling_rights{rhs.castling_rights()};
    if (lhs_castling_rights != rhs_castling_rights) {
        return lhs_castling_rights < rhs_castling_rights;
    }
    // TODO: In operator==() and here, address that the PGN standard requires
    // that EPD and FEN indicate whenever a pawn moved two squares last move
    // (instead of just if an en passant capture is actually possible!), but
    // that two such positions reaching the same destination state are
    // nonetheless equivalent whenever no en passant capture is actually
    // possible in either of them.
    coordinate_type const lhs_en_passant_target{lhs.en_passant_target()};
    coordinate_type const rhs_en_passant_target{rhs.en_passant_target()};
    return lhs_en_passant_target < rhs_en_passant_target;
}


bool operator>(egt_position_type const & lhs, egt_position_type const & rhs) {
    return not (lhs < rhs);
}


bool operator<=(egt_position_type const & lhs, egt_position_type const & rhs) {
    return not (lhs > rhs);
}


bool operator>=(egt_position_type const & lhs, egt_position_type const & rhs) {
    return not (lhs < rhs);
}


std::ostream & operator<<(
    std::ostream & out,
    egt_position_type const & position
) {
    return out << position.as_string();
}


} // namespace representation
