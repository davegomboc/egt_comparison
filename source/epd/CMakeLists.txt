add_library(board_contents_type
  board_contents_type.cpp
  board_contents_type.hpp
)
target_link_libraries(board_contents_type
)

add_library(castling_rights_type
  castling_rights_type.cpp
  castling_rights_type.hpp
)
target_link_libraries(castling_rights_type
  primitive_bit_operations
)

add_library(en_passant_target_type
  en_passant_target_type.cpp
  en_passant_target_type.hpp
)
target_link_libraries(en_passant_target_type
)

add_library(extended_position_description_type
  extended_position_description_type.cpp
  extended_position_description_type.hpp
)
target_link_libraries(extended_position_description_type
  board_contents_type
  castling_rights_type
  en_passant_target_type
  operations_type
  primitive_text_operations
  side_to_move_type
)

add_library(opcode_type
  opcode_type.cpp
  opcode_type.hpp
)
target_link_libraries(opcode_type
)

add_library(operand_type
  operand_type.cpp
  operand_type.hpp
)
target_link_libraries(operand_type
)

add_library(operands_type
  operands_type.cpp
  operands_type.hpp
)
target_link_libraries(operands_type
  operand_type
)

add_library(operation_type
  operation_type.cpp
  operation_type.hpp
)
target_link_libraries(operation_type
  opcode_type
  operands_type
)

add_library(operations_type
  operations_type.cpp
  operations_type.hpp
)
target_link_libraries(operations_type
  operation_type
)

add_library(side_to_move_type
  side_to_move_type.cpp
  side_to_move_type.hpp
)
target_link_libraries(side_to_move_type
)

add_executable(epd_tests
  board_contents_type_test.cpp
  board_contents_type_test.hpp
  castling_rights_type_test.cpp
  castling_rights_type_test.hpp
  en_passant_target_type_test.cpp
  en_passant_target_type_test.hpp
  extended_position_description_type_test.cpp
  extended_position_description_type_test.hpp
  opcode_type_test.cpp
  opcode_type_test.hpp
  operand_type_test.cpp
  operand_type_test.hpp
  operation_type_test.cpp
  operation_type_test.hpp
  operations_type_test.cpp
  operations_type_test.hpp
  side_to_move_type_test.cpp
  side_to_move_type_test.hpp
)

target_link_libraries(epd_tests
  gtest_main
  board_contents_type
  castling_rights_type
  en_passant_target_type
  extended_position_description_type
  opcode_type
  operand_type
  operands_type
  operation_type
  operations_type
  side_to_move_type
)

gtest_discover_tests(epd_tests)
