#if !defined(EGT_COMPARISON_ASSERT_FAIL_HPP)
#define EGT_COMPARISON_ASSERT_FAIL_HPP


extern "C" {

[[noreturn]] void __assert_fail(
    const char * assertion,
    const char * file,
    unsigned int line,
    const char * function);

}


#endif // EGT_COMPARISON_ASSERT_FAIL_HPP
