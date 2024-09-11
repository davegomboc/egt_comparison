#ifndef EGT_COMPARISON_INTEGER_LOGARITHMS_HPP
#define EGT_COMPARISON_INTEGER_LOGARITHMS_HPP


// from https://stackoverflow.com/questions/23781506/
template <typename uint_type>
constexpr unsigned int floor_log2(uint_type const x) {
    return x == 1 ? 0 : 1 + floor_log2(x >> 1);
}

template <typename uint_type>
constexpr unsigned int ceil_log2(uint_type const x) {
    return x == 1 ? 0 : floor_log2(x - 1) + 1;
}

template <typename uint_type>
constexpr unsigned int floor_log10(uint_type const x) {
    return x == 1 ? 0 : 1 + floor_log2(x / 10);
}

template <typename uint_type>
constexpr unsigned ceil_log10(uint_type const x) {
    return x == 1 ? 0 : floor_log10(x - 1) + 1;
}


#endif // EGT_COMPARISON_INTEGER_LOGARITHMS_HPP
