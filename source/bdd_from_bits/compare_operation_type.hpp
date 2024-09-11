#ifndef EGT_COMPARISON_BDD_FROM_BITS_COMPARE_OPERATION_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_COMPARE_OPERATION_TYPE_HPP


#include "bdd_from_bits/bdd_operation_type.hpp"


//
// Compares two nodes P and Q.
//    Returns one of:
//      = : if P == Q
//      ! : if P != Q and neither can be replaced with the other
//      > : if P -> Q, i.e., we can replace undefs in P so that it equals Q
//      < : if P <- Q, i.e., we can replace undefs in Q so that it equals P
//
class compare_operation_type : public bdd_operation_type {
public:
    compare_operation_type(bdd_forest_type & bdd, compute_table_type & ct);

    unsigned char apply(unsigned int P, unsigned int Q);

    static inline unsigned char flip_comparison(bool flip, unsigned char cmp) {
        if (cmp == '<') return flip ? '>' : '<';
        if (cmp == '>') return flip ? '<' : '>';
        return cmp;
    }
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_COMPARE_OPERATION_TYPE_HPP
