#ifndef EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_HPP


#include <cstdio>

#include "bdd_from_bits/file_reader_type.hpp"
#include "bdd_from_bits/parser_type.hpp"


class pla_parser_type : public parser_type {
public:
    pla_parser_type(file_reader_type * fr);
    virtual ~pla_parser_type() override;

    virtual void read_header(
        unsigned int & inbits,
        unsigned int & outbits,
        unsigned int & nmt,
        FILE * debug
    ) override;

    virtual bool read_minterm(char * input_bits, char & out_terminal) override;
private:
    unsigned int inbits;
    unsigned int outbits;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_PLA_PARSER_TYPE_HPP
