#include "irreduce_algorithm.hpp"


template bool irreduce<false, false, false>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<false, false, true>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<false, true, false>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<false, true, true>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<true, false, false>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<true, false, true>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<true, true, false>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);

template bool irreduce<true, true, true>(
    cube_list_tt<32, 8> &,
    indexed_cube_list_tt<cube_list_tt<32, 8>, 30, 30> const &,
    bool,
    bool,
    bool,
    bool,
    std::optional<unsigned long>);
