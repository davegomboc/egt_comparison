cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)
cmake_policy(VERSION 3.13.4)

set(CMAKE_SYSTEM_NAME "Linux" CACHE INTERNAL "-")
set(CMAKE_SYSTEM_VERSION "1" CACHE INTERNAL "-")

set(COMPILER_PREFIX "")
set(COMPILER_SUFFIX "")

set(COMPILER_SYSROOT "/opt/llvm-git")
set(COMPILER_BIN_DIR "${COMPILER_SYSROOT}/bin")
set(COMPILER_INCLUDE_DIR "${COMPILER_SYSROOT}/include/c++/v1")
set(COMPILER_LIB_DIR "${COMPILER_SYSROOT}/lib")
set(COMPILER_LIB_ARCHITECTURE_DIR "${COMPILER_LIB_DIR}/x86_64-unknown-linux-gnu")

unset(CXX_FILESYSTEM_LIBRARY)

set(OS_INCLUDE_DIR "/usr/include")
set(OS_LIB_DIR "/usr/lib")
set(OS_LIB_ARCHITECTURE_DIR "/usr/lib/x86_64-linux-gnu")

set(C_AND_CXX_NONDIAGNOSTIC_FLAGS "-ftabstop=4 -fdiagnostics-show-option -fdiagnostics-color=always -march=native -mtune=native -flto -fno-exceptions -fno-non-call-exceptions")
set(C_AND_CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS "-Wpedantic -Wfatal-errors -Werror -Weverything -Wno-covered-switch-default -Wno-declaration-after-statement -Wno-deprecated -Wno-global-constructors -Wno-missing-declarations -Wno-missing-prototypes -Wno-overlength-strings -Wno-padded -Wno-unsafe-buffer-usage -Wno-error=cast-align -Wno-error=ctad-maybe-unsupported -Wno-error=disabled-macro-expansion -Wno-error=exit-time-destructors -Wno-error=extra-semi-stmt -Wno-error=missing-noreturn -Wno-error=switch-enum -Wno-error=sign-conversion -Wno-error=reserved-id-macro -Wno-error=reserved-identifier -Wno-error=shorten-64-to-32 -Wno-error=undefined-func-template")
set(C_FIRST_PARTY_DIAGNOSTIC_FLAGS "${C_AND_CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS} -Wno-c11-extensions")
set(CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS "${C_AND_CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic")

set(CMAKE_AR "${COMPILER_BIN_DIR}/llvm-ar${COMPILER_SUFFIX}")
set(CMAKE_RANLIB "${COMPILER_BIN_DIR}/llvm-ranlib${COMPILER_SUFFIX}")
set(CMAKE_C_COMPILER "${COMPILER_BIN_DIR}/${COMPILER_PREFIX}clang${COMPILER_SUFFIX}")
set(CMAKE_C_STANDARD_COMPUTED_DEFAULT "-std=c18")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Og -ggdb3 -fdebug-macro -fno-eliminate-unused-debug-types -fno-omit-frame-pointer -fsanitize=address,implicit-conversion,nullability,undefined -fstack-protector-all")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Ofast -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Og -ggdb3 -fdebug-macro -fno-omit-frame-pointer")

set(CMAKE_CXX_COMPILER "${COMPILER_BIN_DIR}/${COMPILER_PREFIX}clang++${COMPILER_SUFFIX}")
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "-std=c++17")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Og -ggdb3 -fdebug-macro -fno-eliminate-unused-debug-types -fno-omit-frame-pointer -fsanitize=address,implicit-conversion,nullability,undefined -fstack-protector-all -D_LIBCPP_DEBUG=1")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Ofast -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Og -ggdb3 -fdebug-macro -fno-omit-frame-pointer")

set(LINKER_FLAGS_COMMON "-L${COMPILER_LIB_DIR}:${COMPILER_LIB_ARCHITECTURE_DIR}:${OS_LIB_DIR}:${OS_LIB_ARCHITECTURE_DIR} -fuse-ld=gold -ltbb")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${LINKER_FLAGS_COMMON} -O0 -fno-eliminate-unused-debug-symbols")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${LINKER_FLAGS_COMMON} -O2")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${LINKER_FLAGS_COMMON} -Ofast")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${LINKER_FLAGS_COMMON} -O0")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${LINKER_FLAGS_COMMON} -O0 -fno-eliminate-unused-debug-symbols")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "${LINKER_FLAGS_COMMON} -O2")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${LINKER_FLAGS_COMMON} -Ofast")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "${LINKER_FLAGS_COMMON} -O0")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${LINKER_FLAGS_COMMON} -O0 -fno-eliminate-unused-debug-symbols")
set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL "${LINKER_FLAGS_COMMON} -O2")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${LINKER_FLAGS_COMMON} -Ofast")
set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "${LINKER_FLAGS_COMMON} -O0")

set(CMAKE_FIND_ROOT_PATH)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set_property(GLOBAL PROPERTY CMAKE_DL_LIBS dl)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
