#ifndef EGT_COMPARISON_BDD_FROM_BITS_EXCEPTION_NEUTRALITY_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_EXCEPTION_NEUTRALITY_HPP


#include <exception>
#include <iostream>


#if !defined(__cpp_exceptions)
[[noreturn]]
#endif
void bdd_terminate(std::string const & message);


#endif // EGT_COMPARISON_BDD_FROM_BITS_EXCEPTION_NEUTRALITY_HPP
