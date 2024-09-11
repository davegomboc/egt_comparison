add_library(bdd_forest_type
  bdd_forest_type.cpp
  bdd_forest_type.hpp
)
target_link_libraries(bdd_forest_type
  bdd_node_type
  compute_table_type
  exception_neutrality
)

add_library(bdd_node_type
  bdd_node_type.cpp
  bdd_node_type.hpp
)
target_link_libraries(bdd_node_type
)

add_library(bdd_operation_type
  bdd_operation_type.cpp
  bdd_operation_type.hpp
)
target_link_libraries(bdd_operation_type
  bdd_forest_type
  compute_table_type
)

add_library(bin_parser_type
  bin_parser_type.cpp
  bin_parser_type.hpp
)
target_link_libraries(bin_parser_type
  parser_type
)

add_library(build_bdd
  build_bdd.cpp
  build_bdd.hpp
)
target_link_libraries(build_bdd
  exception_neutrality
  minterm_buffer_type
  one_sided_matching_resolver_type
  resolver_type
  restrict_resolver_type
  two_sided_matching_resolver_type
)

add_library(command_line_argument_processing
  command_line_argument_processing.cpp
  command_line_argument_processing.hpp
)
target_link_libraries(command_line_argument_processing
)

add_library(common_operation_type
  common_operation_type.cpp
  common_operation_type.hpp
)
target_link_libraries(common_operation_type
  bdd_forest_type
  compute_table_type
)

add_library(compare_operation_type
  compare_operation_type.cpp
  compare_operation_type.hpp
)
target_link_libraries(compare_operation_type
  bdd_operation_type
)

add_library(compute_table_type
  compute_table_type.hpp
  compute_table_type.cpp
)
target_link_libraries(compute_table_type
  bdd_operation_type
  exception_neutrality
)

add_library(exception_neutrality
  exception_neutrality.hpp
  exception_neutrality.cpp
)
target_link_libraries(exception_neutrality
)

add_library(file_reader_type
  file_reader_type.cpp
  file_reader_type.hpp
)
target_link_libraries(file_reader_type
)

add_library(fixed_decision_diagram_type
  fixed_decision_diagram_type.cpp
  fixed_decision_diagram_type.hpp
)
target_link_libraries(fixed_decision_diagram_type
  exception_neutrality
)

add_library(has_common_operation_type
  has_common_operation_type.cpp
  has_common_operation_type.hpp
)
target_link_libraries(has_common_operation_type
  bdd_forest_type
  compute_table_type
)

add_library(minterm_buffer_type
  minterm_buffer_type.cpp
  minterm_buffer_type.hpp
)
target_link_libraries(minterm_buffer_type
  bdd_forest_type
  resolver_type
)

add_library(one_sided_matching_operation_type
  one_sided_matching_operation_type.cpp
  one_sided_matching_operation_type.hpp
)
target_link_libraries(one_sided_matching_operation_type
  bdd_operation_type
  compare_operation_type
)

add_library(one_sided_matching_resolver_type
  one_sided_matching_resolver_type.cpp
  one_sided_matching_resolver_type.hpp
)
target_link_libraries(one_sided_matching_resolver_type
  bdd_forest_type
  compare_operation_type
  compute_table_type
  one_sided_matching_operation_type
  resolver_type
)

add_library(parser_type
  parser_type.cpp
  parser_type.hpp
)
target_link_libraries(parser_type
  exception_neutrality
  file_reader_type
  bin_parser_type
  pla_parser_type
)

add_library(pla_parser_type
  pla_parser_type.cpp
  pla_parser_type.hpp
)
target_link_libraries(pla_parser_type
  parser_type
)

add_library(read_bdd
  read_bdd.cpp
  read_bdd.hpp
)
target_link_libraries(read_bdd
)

add_library(resolver_type
  resolver_type.cpp
  resolver_type.hpp
)
target_link_libraries(resolver_type
  bdd_forest_type
  compute_table_type
)

add_library(restrict_operation_type
  restrict_operation_type.cpp
  restrict_operation_type.hpp
)
target_link_libraries(restrict_operation_type
  bdd_operation_type
  bdd_forest_type
  compute_table_type
)

add_library(restrict_resolver_type
  restrict_resolver_type.cpp
  restrict_resolver_type.hpp
)
target_link_libraries(restrict_resolver_type
  bdd_forest_type
  compute_table_type
  restrict_operation_type
  resolver_type
)

add_library(two_sided_matching_operation_type
  two_sided_matching_operation_type.cpp
  two_sided_matching_operation_type.hpp
)
target_link_libraries(two_sided_matching_operation_type
  bdd_operation_type
  compare_operation_type
)

add_library(two_sided_matching_resolver_type
  two_sided_matching_resolver_type.cpp
  two_sided_matching_resolver_type.hpp
)
target_link_libraries(two_sided_matching_resolver_type
  bdd_forest_type
  common_operation_type
  compute_table_type
  has_common_operation_type
  two_sided_matching_operation_type
  resolver_type
)

add_executable(build_bdd_main
  build_bdd_main.cpp
  build_bdd_main.hpp
)
target_link_libraries(build_bdd_main
  build_bdd
  command_line_argument_processing
  compute_table_type
  exception_neutrality
  parser_type
)

add_executable(read_bdd_main
  read_bdd_main.cpp
  read_bdd_main.hpp
)
target_link_libraries(read_bdd_main
  fixed_decision_diagram_type
)

add_executable(read_bdds_main
  read_bdds_main.cpp
  read_bdds_main.hpp
)
target_link_libraries(read_bdds_main
  fixed_decision_diagram_type
)

add_executable(test_parser_main
  test_parser_main.cpp
  test_parser_main.hpp
)
target_link_libraries(test_parser_main
  exception_neutrality
  parser_type
)

add_executable(bdd_from_bits_tests
  fixed_decision_diagram_type_test.cpp
  fixed_decision_diagram_type_test.hpp
)

target_link_libraries(bdd_from_bits_tests
  gtest_main
  fixed_decision_diagram_type
)

gtest_discover_tests(bdd_from_bits_tests)
