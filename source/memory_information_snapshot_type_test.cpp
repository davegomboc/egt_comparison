#include "memory_information_snapshot_type_test.hpp"


#include <algorithm>
#include <fstream>
#include <iostream>


TEST(MemoryInformationSnapshotType, DecodeNoSuffix) {
    std::string_view const original("0");
    std::string_view modifiable(original);
    std::size_t const multiplier{decode_proc_meminfo_suffix(modifiable)};
    EXPECT_EQ(1, multiplier);
    EXPECT_EQ(original, modifiable);
}


TEST(MemoryInformationSnapshotType, DecodeSuffixMissingPrecedingSpace) {
    std::string_view const original("123kB");
    std::string_view modifiable(original);
    std::size_t const multiplier{decode_proc_meminfo_suffix(modifiable)};
    EXPECT_EQ(1, multiplier);
    EXPECT_EQ(original, modifiable);
}


TEST(MemoryInformationSnapshotType, DecodeSuffixkB) {
    std::string_view const original("34359738367 kB");
    std::string_view modifiable(original);
    std::size_t const multiplier{decode_proc_meminfo_suffix(modifiable)};
    EXPECT_EQ(1024, multiplier);
    EXPECT_EQ("34359738367", modifiable);
}


TEST(MemoryInformationSnapshotType, PrimaryKeyPrecedesAllOthers) {
    std::string const primary_key{memory_information_snapshot_type::primary_key()};
    std::unordered_map<std::string, std::size_t> memory_information;
    memory_information["Xyz"] = 242526;
    memory_information["Uvw"] = 333333;
    memory_information["Def"] = 7331;
    memory_information["Abc"] = 999999;
    memory_information[primary_key] = as_size_t(std::chrono::steady_clock::now());
    std::vector<std::string> const memory_information_keys{keys(memory_information)};
    EXPECT_EQ(primary_key, memory_information_keys[0]);
}


TEST(MemoryInformationSnapshotType, PrimaryKeyIsPresentJustOnce) {
    std::string const primary_key{memory_information_snapshot_type::primary_key()};
    std::unordered_map<std::string, std::size_t> memory_information;
    memory_information["Xyz"] = 242526;
    memory_information["Uvw"] = 333333;
    memory_information["Def"] = 7331;
    memory_information["Abc"] = 999999;
    memory_information[primary_key] = as_size_t(std::chrono::steady_clock::now());
    std::vector<std::string> const memory_information_keys{keys(memory_information)};
    std::size_t primary_key_occurence_count{};
    for (std::string const & key: memory_information_keys) {
        if (primary_key == key) {
            ++primary_key_occurence_count;
        }
    }
    EXPECT_EQ(1, primary_key_occurence_count);
}


TEST(MemoryInformationSnapshotType, NonprimaryKeysAreSortedLexicographically) {
    std::string const primary_key{memory_information_snapshot_type::primary_key()};
    std::unordered_map<std::string, std::size_t> memory_information;
    memory_information["Xyz"] = 242526;
    memory_information["Uvw"] = 333333;
    memory_information["Def"] = 7331;
    memory_information["Abc"] = 999999;
    memory_information[primary_key] = as_size_t(std::chrono::steady_clock::now());
    std::vector<std::string> const memory_information_keys{keys(memory_information)};
    EXPECT_TRUE(std::is_sorted(std::next(memory_information_keys.cbegin()),
     memory_information_keys.cend()));
}


TEST(MemoryInformationSnapshotType, ParseSampleMemAvailableLine) {
    std::string_view line("MemAvailable:   489263288 kB");
    std::optional<std::pair<std::string, std::size_t>> key_value_pair_opt{
     parse_line_opt(line)};
    ASSERT_NE(std::nullopt, key_value_pair_opt);
    EXPECT_EQ("MemAvailable", key_value_pair_opt->first);
    EXPECT_EQ(1024ull * 489263288ull, key_value_pair_opt->second);
}


TEST(MemoryInformationSnapshotType, ParseSampleBounceLine) {
    std::string_view line("Bounce:                0 kB");
    std::optional<std::pair<std::string, std::size_t>> key_value_pair_opt{
     parse_line_opt(line)};
    ASSERT_NE(std::nullopt, key_value_pair_opt);
    EXPECT_EQ("Bounce", key_value_pair_opt->first);
    EXPECT_EQ(0, key_value_pair_opt->second);
}


TEST(MemoryInformationSnapshotType, ParseSampleHugePagesTotalLine) {
    std::string_view line("HugePages_Total:       0");
    std::optional<std::pair<std::string, std::size_t>> key_value_pair_opt{
     parse_line_opt(line)};
    ASSERT_NE(std::nullopt, key_value_pair_opt);
    EXPECT_EQ("HugePages_Total", key_value_pair_opt->first);
    EXPECT_EQ(0, key_value_pair_opt->second);
}


TEST(MemoryInformationSnapshotType, ParseProcMeminfo) {
    std::ifstream data_in("/proc/meminfo");
    std::optional<std::unordered_map<std::string, std::size_t>> meminfo_opt{
     parse_opt(data_in)};
    ASSERT_NE(std::nullopt, meminfo_opt);
    auto const & keys_and_values{*meminfo_opt};
    for (auto const & kv: keys_and_values) {
        std::cout << "|" << kv.first << "| => |" << kv.second << "|.\n";
    }
    std::cout << std::flush;
}


TEST(MemoryInformationSnapshotType, CaptureOnly) {
    memory_information_snapshot_type snapshot;
    EXPECT_FALSE(snapshot.empty());
    EXPECT_FALSE(snapshot.parsed());
    EXPECT_EQ(std::nullopt, snapshot.parsed_data_opt());
}


TEST(MemoryInformationSnapshotType, CaptureAndProcess) {
    memory_information_snapshot_type snapshot;
    snapshot.parse();
    EXPECT_TRUE(snapshot.parsed());
    ASSERT_NE(std::nullopt, snapshot.parsed_data_opt());
    auto const & keys_and_values{*(snapshot.parsed_data_opt())};
    for (auto const & kv: keys_and_values) {
        std::cout << "|" << kv.first << "| => |" << kv.second << "|.\n";
    }
    std::cout << std::flush;
}
