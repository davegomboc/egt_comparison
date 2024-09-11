#ifndef EGT_COMPARISON_BDD_FROM_BITS_PARSER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_PARSER_TYPE_HPP


#include "bdd_from_bits/file_reader_type.hpp"


//
// Abstract parser interface
//
class parser_type {
public:
    parser_type(file_reader_type * fr);

    virtual ~parser_type();

    /*
        Build a parser, reading from stdin, with specified file
        compression and file format.

	Format choices:
            'p': pla file
            'b': bin file

	Compression choices:
            ' ': none
            'x': xz
            'g': gz
            'b': bz2

        Returns nullptr on error.
    */
    static parser_type * new_parser(char format, char compression);

    /*
        Build a parser, reading from a pathname,

        Returns nullptr on error.
    */
    static parser_type * new_parser(char const * pathname);

    virtual void read_header(
        unsigned int & inbits,
        unsigned int & outbits,
        unsigned int & minterms,
        FILE * debug = nullptr) = 0;

    // Returns true on successful read, false otherwise.
    virtual bool read_minterm(char * input_bits, char & out_terminal) = 0;

    void debug(bool show_header, bool show_minterms, bool show_summary);

protected:
    inline bool eof() {
        return FR->eof();
    }

    inline int get() {
        return FR->get();
    }

    inline void unget(int c) {
        FR->unget(c);
    }

    inline unsigned read_unsigned() {
        return FR->read_unsigned();
    }

    inline int skip_until(char x) {
        for (;;) {
            int c = get();
            if (x == c)   return x;
            if (EOF == c) return EOF;
        }
    }

    inline unsigned read(void * ptr, unsigned bytes) {
        return FR->read(ptr, bytes);
    }

private:
    static parser_type * new_parser(
        const char * pathname,
        char format,
        char compression);

    file_reader_type * FR;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_PARSER_TYPE_HPP
