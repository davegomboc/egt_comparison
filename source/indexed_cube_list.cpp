#include "indexed_cube_list.hpp"

#include "cube_list.hpp"


// historical default
template struct cube_list_index_range_tt<30>;

// historical default
template class indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30>;

// 2-figurine
template class indexed_cube_list_tt<cube_list_tt<13, 3>, 30, 30>;
// 3-figurine
template class indexed_cube_list_tt<cube_list_tt<19, 3>, 30, 30>;
// 4-figurine
template class indexed_cube_list_tt<cube_list_tt<25, 3>, 30, 30>;
// 5-figurine
template class indexed_cube_list_tt<cube_list_tt<31, 3>, 30, 30>;
