#if !defined(EGT_COMPARISON_MEMORY_INFORMATION_SNAPSHOTTER_TYPE_HPP)
#define EGT_COMPARISON_MEMORY_INFORMATION_SNAPSHOTTER_TYPE_HPP


#include <ostream>
#include <string>
#include <vector>

#include "memory_information_snapshot_type.hpp"


class memory_information_snapshotter_type {
public:
    memory_information_snapshotter_type();

    void take_snapshot();

    // Returns true iff an error occurred.
    bool parse_all_snapshots();

    void emit_tab_separated_header(std::ostream & data_out);

    // Returns true iff an error occurred.
    bool emit_tab_separated_values(std::ostream & data_out);

private:
    std::vector<std::string> the_snapshot_keys;
    std::vector<memory_information_snapshot_type> the_snapshots;
};


#endif // EGT_COMPARISON_MEMORY_INFORMATION_SNAPSHOTTER_TYPE_HPP
