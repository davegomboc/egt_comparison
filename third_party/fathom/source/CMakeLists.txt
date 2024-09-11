add_library(fathom_argv_processing
  fathom_argv_processing.cpp
  fathom_argv_processing.hpp
)

target_link_libraries(fathom_argv_processing
  prober_type
)

add_library(probe_report_enum
  probe_report_enum.cpp
  probe_report_enum.hpp
)

target_link_libraries(probe_report_enum
  operand_type
  operands_type
  operation_type
)

add_library(probe_report_type
  probe_report_type.cpp
  probe_report_type.hpp
)

target_link_libraries(probe_report_type
  probe_report_enum
)

add_library(prober_type
  prober_type.cpp
  prober_type.hpp
)

target_link_libraries(prober_type
  probing
)

add_library(probing
  probing.cpp
  probing.hpp
)

target_link_libraries(probing
  tb_position_type
)

add_library(tb_position_type
  tb_position_type.cpp
  tb_position_type.hpp
)

target_link_libraries(tb_position_type
  extended_position_description_type
  probe_report_enum
  tbprobe
)

add_library(tbprobe
  tbprobe.c
  tbprobe.h
)

target_link_libraries(tbprobe
)

add_executable(fathom
  fathom.cpp
  fathom.hpp
)

target_link_libraries(fathom
  fathom_argv_processing
  probing
  probe_report_enum
  tbprobe
  tb_position_type
)

add_executable(egt_probe
  egt_probe.cpp
)

target_link_libraries(egt_probe
  fathom_argv_processing
  probing
  probe_report_enum
)

add_executable(fathom_tests
  egt_probe_test.cpp
  egt_probe_test.hpp
  fathom_test.cpp
  fathom_test.hpp
  prober_type_test.cpp
  prober_type_test.hpp
  probing_test.cpp
  probing_test.hpp
  tb_position_type_test.cpp
  tb_position_type_test.hpp
)

target_link_libraries(fathom_tests
  gtest_main
  extended_position_description_type
  unxz
  probe_report_type
  prober_type
  probing
  tb_position_type
)

gtest_discover_tests(fathom_tests)
