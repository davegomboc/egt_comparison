#ifndef EGT_COMPARISON_BDD_FROM_BITS_MINTERM_BUFFER_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_MINTERM_BUFFER_HPP


class bdd_forest_type;
class resolver_type;


class minterm_buffer_type {
public:
    minterm_buffer_type(bdd_forest_type & bdd, unsigned int & root, unsigned int bufsize, resolver_type * R);

    ~minterm_buffer_type();

    // Returns true if we had to flush the buffer.
    bool add(char const * linebuf, char term);
    void flush();

    inline unsigned long get_num_minterms() const { return minterms; }

private:
    unsigned short top_changed(char const * state1, char const * state2) const;

    bdd_forest_type & BDD;
    unsigned int & root;

    char ** statebuf;
    unsigned int const bufsize;
    unsigned int bufptr;
    unsigned long minterms;

    resolver_type * R;
    char const * last_add;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_MINTERM_BUFFER_HPP
