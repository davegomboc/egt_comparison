#include "memory_information_snapshotter_type_test.hpp"


#include <chrono>
#include <sstream>
#include <thread>


TEST(MemoryInformationSnapshotterType, Initialization) {
    memory_information_snapshotter_type snapshotter;
    std::ostringstream data_out;
    snapshotter.emit_tab_separated_header(data_out);
    EXPECT_FALSE(snapshotter.emit_tab_separated_values(data_out));
}


TEST(MemoryInformationSnapshotterType, TakingSnapshots) {
    memory_information_snapshotter_type snapshotter;
    for (std::size_t i{}; i != 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        snapshotter.take_snapshot();
    }
    std::ostringstream data_out;
    snapshotter.emit_tab_separated_header(data_out);
    EXPECT_FALSE(snapshotter.emit_tab_separated_values(data_out));
}
