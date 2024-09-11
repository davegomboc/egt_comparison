cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)
cmake_policy(VERSION 3.13.4)
project(egt_comparison C CXX)

set(CMAKE_COLOR_MAKEFILE TRUE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

if (NOT DEFINED "${GIT_TOP_DIR}")
  set(GIT_TOP_DIR "$ENV{GIT_TOP_DIR}")
endif()
if (NOT IS_DIRECTORY "${GIT_TOP_DIR}")
  message(FATAL_ERROR "The CMake variable GIT_TOP_DIR, currently |${GIT_TOP_DIR}|, must be set correctly.")
endif()

if (NOT (DEFINED CMAKE_TOOLCHAIN_FILE))
  message(FATAL_ERROR "Use of -DCMAKE_TOOLCHAIN_FILE=... is required.")
endif()

include(CTest)
if (NOT TARGET check)
  add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
endif()

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Google Test.
FetchContent_Declare(
  google_test
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG "96eadf659fb75ecda943bd97413c71d4c17c4f43" # HEAD as of 2023-12-22.
  GIT_PROGRESS TRUE
  GIT_SHALLOW FALSE  # Doesn't work when using an arbitrary hash.
)
FetchContent_GetProperties(google_test)
if (NOT google_test_POPULATED)
  FetchContent_Populate(google_test)
  add_subdirectory("${google_test_SOURCE_DIR}" "${google_test_BINARY_DIR}")
endif()
enable_testing()
include(GoogleTest)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${C_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${C_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${C_FIRST_PARTY_DIAGNOSTIC_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/source")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third_party/fathom/source")
add_subdirectory(source)
add_subdirectory(third_party/fathom/source)
