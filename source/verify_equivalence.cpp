#include "verify_equivalence.hpp"


#include "cube_list.hpp"
#include "indexed_cube_list.hpp"


using cube_list_type = cube_list_tt<32, 8>;
using indexed_cube_list_type = indexed_cube_list_tt<cube_list_type, 30, 30>;


template bool verify_equivalence<
    cube_list_type,
    cube_list_type,
    false
>(cube_list_type const &, cube_list_type const &);

template bool verify_equivalence<
    cube_list_type,
    cube_list_type,
    true
>(cube_list_type const &, cube_list_type const &);

template bool verify_equivalence<
    indexed_cube_list_type,
    cube_list_type,
    false
>(indexed_cube_list_type const &, cube_list_type const &);

template bool verify_equivalence<
    indexed_cube_list_type,
    cube_list_type,
    true
>(indexed_cube_list_type const &, cube_list_type const &);

template bool verify_equivalence<
    cube_list_type,
    indexed_cube_list_type,
    false
>(cube_list_type const &, indexed_cube_list_type const &);

template bool verify_equivalence<
    cube_list_type,
    indexed_cube_list_type,
    true
>(cube_list_type const &, indexed_cube_list_type const &);

template bool verify_equivalence<
    indexed_cube_list_type,
    indexed_cube_list_type,
    false
>(indexed_cube_list_type const &, indexed_cube_list_type const &);

template bool verify_equivalence<
    indexed_cube_list_type,
    indexed_cube_list_type,
    true
>(indexed_cube_list_type const &, indexed_cube_list_type const &);
