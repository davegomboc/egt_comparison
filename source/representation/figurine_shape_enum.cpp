#include "representation/figurine_shape_enum.hpp"


#include <algorithm>
#include <array>
#include <cassert>
#include <ostream>


namespace representation {


bool next(figurine_shape_enum & shape) {
    assert(valid(shape));
    if (figurine_shape_enum::pawn == shape) {
        shape = figurine_shape_enum::king;
        return false;
    }
    shape = static_cast<figurine_shape_enum>(static_cast<unsigned>(shape) + 1);
    return true;
}


bool prev(figurine_shape_enum & shape) {
    assert(valid(shape));
    if (figurine_shape_enum::king == shape) {
        shape = figurine_shape_enum::pawn;
        return false;
    }
    shape = static_cast<figurine_shape_enum>(static_cast<unsigned>(shape) - 1);
    return true;
}


figurine_shape_enum & operator++(figurine_shape_enum & shape) {
    next(shape);
    return shape;
}


figurine_shape_enum & operator--(figurine_shape_enum & shape) {
    prev(shape);
    return shape;
}


logic_digits_type as_logic_digits(figurine_shape_enum const shape) {
    assert(valid(shape));
    logic_digits_type result;
    result.push_back(as_index(shape) & 0b0100 ? on : off);
    result.push_back(as_index(shape) & 0b0010 ? on : off);
    result.push_back(as_index(shape) & 0b0001 ? on : off);
    return result;
}


// k q r b n p
logic_digits_type as_one_hot_logic_digits(figurine_shape_enum const shape) {
    assert(valid(shape));
    logic_digits_type result;
    result.push_back(shape == figurine_shape_enum::king ? on : off);
    result.push_back(shape == figurine_shape_enum::queen ? on : off);
    result.push_back(shape == figurine_shape_enum::rook ? on : off);
    result.push_back(shape == figurine_shape_enum::bishop ? on : off);
    result.push_back(shape == figurine_shape_enum::knight ? on : off);
    result.push_back(shape == figurine_shape_enum::pawn ? on : off);
    return result;
}


std::ostream & operator<<(std::ostream & out, figurine_shape_enum const shape) {
    assert(valid(shape));
    return out.put(as_char(shape));
}


} // namespace representation
