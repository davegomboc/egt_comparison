#ifndef EGT_COMPARISON_BDD_FROM_BITS_COMPUTE_TABLE_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_COMPUTE_TABLE_TYPE_HPP


#include <cassert>


class bdd_operation_type;


class compute_table_type {
public:
    compute_table_type(unsigned int size);
    ~compute_table_type();

    // Note: op = 0 is reserved for CT use.
    unsigned char register_op(bdd_operation_type * op);
    void unregister_op(bdd_operation_type * op);

    void remove_stales(unsigned short seqno);

    // For unary operations

    bool find_unary(unsigned char op, unsigned int P, unsigned int & ans) const;
    void add_unary(unsigned char op, unsigned int P, unsigned int ans);

    // For binary operations,
    // or sequenced unary operations.

    bool find_binary(unsigned char op, unsigned int P, unsigned int Q, unsigned int & ans) const;
    void add_binary(unsigned char op, unsigned int P, unsigned int Q, unsigned int ans);

private:
    // For unary operations

    inline bool equals(unsigned int H, unsigned char op, unsigned int P) const {
        return (oparray[H] == op) && (Parray[H] == P);
    }

    inline void set(unsigned int H, unsigned char op, unsigned int P, unsigned int R) {
        assert(H < size);
        oparray[H] = op;
        Parray[H] = P;
        Rarray[H] = R;
    }

    // For binary operations

    inline bool equals(unsigned int H, unsigned char op, unsigned int P, unsigned int Q) const {
        return (oparray[H] == op) and (Parray[H] == P) and (Qarray[H] == Q);
    }

    inline void set(unsigned int H, unsigned char op, unsigned int P, unsigned int Q, unsigned int R) {
        assert(H < size);
        oparray[H] = op;
        Parray[H] = P;
        Qarray[H] = Q;
        Rarray[H] = R;
    }

private:
    unsigned int size;

    unsigned char * oparray;
    unsigned int * Parray;
    unsigned int * Qarray;
    unsigned int * Rarray;

    bdd_operation_type * optable[256];
    // Status of each operation slot
    //  0: unused
    //  1: registered and active
    //  2: unregistered, but not removed yet
    char opstatus[256];
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_COMPUTE_TABLE_TYPE_HPP
