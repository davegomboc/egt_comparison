#if !defined(FATHOM_ARGV_PROCESSING_HPP)
#define FATHOM_ARGV_PROCESSING_HPP


#include <iosfwd>
#include <utility>

#include "prober_type.hpp"


void print_help(std::ostream & out, char const * const prog);

std::pair<bool, prober_type const>
shared_setup_for_mains(int argc, char * * argv);


#endif // FATHOM_ARGV_PROCESSING_HPP
