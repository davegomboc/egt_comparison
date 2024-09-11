#include "bdd_from_bits/compute_table_type.hpp"


#include <cstring>

#include "bdd_from_bits/bdd_operation_type.hpp"
#include "bdd_from_bits/exception_neutrality.hpp"


inline void hash(unsigned int & h, unsigned char b) {
    h = (h*31) ^ b;
}

inline void hash(unsigned int & h, unsigned int b) {
    h = (h*31) ^ ((b & 0xff000000) >> 24);
    h = (h*31) ^ ((b & 0x00ff0000) >> 16);
    h = (h*31) ^ ((b & 0x0000ff00) >> 8);
    h = (h*31) ^ (b & 0x000000ff);
}

inline unsigned int hash(unsigned char b, unsigned int P) {
    unsigned int h = 0xdeadbeef;
    hash(h, b);
    hash(h, P);
    return h;
}

inline unsigned int hash(unsigned char b, unsigned int P, unsigned int Q) {
    unsigned int h = 0xdeadbeef;
    hash(h, b);
    hash(h, P);
    hash(h, Q);
    return h;
}

compute_table_type::compute_table_type(unsigned int sz) : size(sz) {
    oparray = new unsigned char[size];
    Parray = new unsigned int[size];
    Qarray = new unsigned int[size];
    Rarray = new unsigned int[size];

    bzero(oparray, size);

    for (unsigned int i = 0; i < 256; i++) {
        optable[i] = nullptr;
        opstatus[i] = 0;
    }
}

compute_table_type::~compute_table_type() {
    delete[] oparray;
    delete[] Parray;
    delete[] Qarray;
    delete[] Rarray;

    for (unsigned int u = 0; u < 256; u++) delete optable[u];
}

unsigned char compute_table_type::register_op(bdd_operation_type * op) {
    // sanity check
    if (nullptr == op) return 0;

    for (unsigned int u = 1; u < 256; u++) {
        if (optable[u] == op) {
            opstatus[u] = 1;
            return static_cast<unsigned char>(u);
        }
    }
    for (unsigned int u = 1; u < 256; u++) {
        if (opstatus[u]) continue;
        optable[u] = op;
        opstatus[u] = 1;
        return static_cast<unsigned char>(u);
    }
    bdd_terminate("Operation table full");
    __builtin_unreachable();
    // return 0;
}

void compute_table_type::unregister_op(bdd_operation_type * op) {
    // sanity check
    if (nullptr == op) return;

    for (unsigned int u = 1; u < 256; u++) {
        if (optable[u] == op) {
            opstatus[u] = 2;
            optable[u] = nullptr;
            return;
        }
    }
}

void compute_table_type::remove_stales(unsigned short seqno) {
    for (unsigned int u = 0; u < size; u++) {
        unsigned char opind = oparray[u];
        if (0==opind) continue;
        if (opstatus[opind]!=1) {
            oparray[u] = 0;
            continue;
        }
        bdd_operation_type const * op = optable[oparray[u]];
        assert(op);
        if (op->entry_is_stale(seqno, Parray[u], Qarray[u], Rarray[u])) {
            oparray[u] = 0;
        }
    }
    // Any unregistered ops can be cleared
    for (unsigned int u = 0; u < 256; u++) {
        if (opstatus[u] == 2) {
            opstatus[u] = 0;
        }
    }
}


//
// Unary
//

bool compute_table_type::find_unary(unsigned char op, unsigned int P, unsigned int & ans) const {
    unsigned int H;
    // check H, H+1, H+2

    if (equals(H = hash(op, P) % size, op, P)) {
        ans = Rarray[H];
        return true;
    }

    if (equals(H = (H+1) % size, op, P)) {
       ans = Rarray[H];
        return true;
    }

    if (equals(H = (H+1) % size, op, P)) {
        ans = Rarray[H];
        return true;
    }

    return false;
}

void compute_table_type::add_unary(unsigned char op, unsigned int P, unsigned int ans) {
  unsigned int const H0 = hash(op, P) % size;

  unsigned int H = H0;
  // check H, H+1, H+2 for empty.  If none, use H.

  if (0 == oparray[H]) {
      return set(H, op, P, ans);
  }

  H = (H+1) % size;

  if (0 == oparray[H]) {
      return set(H, op, P, ans);
  }

  H = (H+1) % size;

  if (0 == oparray[H]) {
      return set(H, op, P, ans);
  }

  set(H0, op, P, ans);
}


//
// Binary
//

bool compute_table_type::find_binary(unsigned char op, unsigned int P, unsigned int Q, unsigned int & ans) const {
    unsigned int H;
    // check H, H+1, H+2

    if (equals(H = hash(op, P, Q) % size, op, P, Q)) {
        ans = Rarray[H];
        return true;
    }

    if (equals (H = (H+1) % size, op, P, Q)) {
        ans = Rarray[H];
        return true;
    }

    if (equals (H = (H+1) % size, op, P, Q)) {
        ans = Rarray[H];
        return true;
    }

    return false;
}

void compute_table_type::add_binary(unsigned char op, unsigned int P, unsigned int Q, unsigned int ans) {
    unsigned int const H0 = hash(op, P, Q) % size;

    unsigned int H = H0;
    // check H, H+1, H+2 for empty.  If none, use H.

    if (0 == oparray[H]) {
        return set(H, op, P, Q, ans);
    }

    H = (H+1) % size;

    if (0==oparray[H]) {
        return set(H, op, P, Q, ans);
    }

    H = (H+1) % size;

    if (0==oparray[H]) {
        return set(H, op, P, Q, ans);
    }

    set(H0, op, P, Q, ans);
}
