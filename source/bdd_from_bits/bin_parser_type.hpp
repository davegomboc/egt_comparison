#ifndef EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_HPP


#include "bdd_from_bits/file_reader_type.hpp"
#include "bdd_from_bits/parser_type.hpp"


class bin_parser_type : public parser_type {
public:
    bin_parser_type(file_reader_type * fr);

    virtual ~bin_parser_type() override;

    virtual void read_header(
        unsigned int & inbits,
        unsigned int & outbits,
        unsigned int & nmt,
        FILE * debug) override;

    virtual bool read_minterm(char * input_bits, char & out_terminal) override;

private:
    void check_bitstream();
    void clear_bitstream();
    bool read_bit1();
    bool read_bit2();
    bool read_le(unsigned long & u);

    unsigned int inbits;
    unsigned long numf;

    unsigned long bits;
    unsigned int bits_avail;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_BIN_PARSER_TYPE_HPP
