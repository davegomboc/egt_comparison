#include "memory_information_snapshotter_type.hpp"


#include <algorithm>
#include <cassert>
#include <iostream>


memory_information_snapshotter_type::memory_information_snapshotter_type()
  : the_snapshots(1)
{
    std::optional<std::vector<std::string>> the_snapshot_keys_opt{
     the_snapshots[0].keys_opt()};
    assert(std::nullopt != the_snapshot_keys_opt);
    the_snapshot_keys = std::move(*the_snapshot_keys_opt);
    assert(std::is_sorted(std::next(the_snapshot_keys.cbegin()),
     the_snapshot_keys.cend()));
}


void memory_information_snapshotter_type::take_snapshot() {
    the_snapshots.emplace_back();
}


bool memory_information_snapshotter_type::parse_all_snapshots() {
    bool result{};
    for (memory_information_snapshot_type & snapshot: the_snapshots) {
        result |= snapshot.parse();
    }
    return result;
}


void memory_information_snapshotter_type::emit_tab_separated_header(
    std::ostream & data_out
) {
    bool primary_key_emitted{};
    for (std::string const & key: the_snapshot_keys) {
        if (primary_key_emitted) {
            data_out.put('\t');
        } else {
            assert(key == memory_information_snapshot_type::primary_key());
            primary_key_emitted = true;
        }
        data_out << key;
    }
    data_out.put('\n');
}


bool memory_information_snapshotter_type::emit_tab_separated_values(
    std::ostream & data_out
) {
    bool result{parse_all_snapshots()};
    if (result) {
        std::cerr << "EE: Some error occurred when parsing some snapshot.\n";
        return result;
    }
    for (std::size_t i{}; i != the_snapshots.size(); ++i) {
        auto & parsed_data_opt{the_snapshots[i].parsed_data_opt()};
        if (std::nullopt == parsed_data_opt) {
            result = true;
            std::cerr << "EE: Missing parse data for snapshot #" << i << ".\n";
            continue;
        }
        auto const & memory_information{*parsed_data_opt};
        bool primary_key_emitted{};
        for (std::string const & key: the_snapshot_keys) {
            if (primary_key_emitted) {
                data_out.put('\t');
            } else {
                assert(key == memory_information_snapshot_type::primary_key());
                primary_key_emitted = true;
            }
            auto const & iter{memory_information.find(key)};
            assert(memory_information.cend() != iter);
            data_out << iter->second;
        }
        data_out.put('\n');
    }
    return result;
}
