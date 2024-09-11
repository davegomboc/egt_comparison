cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)
cmake_policy(VERSION 3.13.4)

set(CMAKE_SYSTEM_NAME "Linux" CACHE INTERNAL "-")
set(CMAKE_SYSTEM_VERSION "1" CACHE INTERNAL "-")

set(COMPILER_ARCHITECTURE_SPECIFICATION "x86_64-pc-linux-gnu")
set(COMPILER_PREFIX "${COMPILER_ARCHITECTURE_SPECIFICATION}-")
set(COMPILER_SUFFIX "")
set(COMPILER_VERSION "14.1.0")

set(COMPILER_SYSROOT "/opt/gcc-git")
set(COMPILER_BIN_DIR "${COMPILER_SYSROOT}/bin")
set(COMPILER_LIB_DIR "${COMPILER_SYSROOT}/lib/gcc/${COMPILER_ARCHITECTURE_SPECIFICATION}/${COMPILER_VERSION}")
set(COMPILER_LIB64_DIR "${COMPILER_SYSROOT}/lib64")
set(COMPILER_C_INCLUDE_DIR "${COMPILER_LIB_DIR}/include")
set(COMPILER_CXX_INCLUDE_DIR "${COMPILER_SYSROOT}/include/c++/${COMPILER_VERSION}")

unset(CXX_FILESYSTEM_LIBRARY)

set(OS_INCLUDE_DIR "/usr/include")

set(C_AND_CXX_NONDIAGNOSTIC_FLAGS "-D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -ftabstop=4 -fdiagnostics-show-option -fdiagnostics-color=always -march=native -mtune=native -flto -flto-partition=one -flto-odr-type-merging -ffat-lto-objects -fuse-linker-plugin -fdelete-dead-exceptions -fexceptions -fno-non-call-exceptions -fstrict-flex-arrays=3")
set(C_AND_CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS "-Wpedantic -Wfatal-errors -Werror -Wall -Wextra -Wdouble-promotion -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wfree-nonheap-object -Winit-self -Wmissing-include-dirs -Wtrivial-auto-var-init -Wno-error=alloc-size-larger-than=PTRDIFF_MAX -Wconversion -Wsign-conversion -Wshadow=local -Wno-error=format-overflow -Wno-error=null-dereference -Wno-error=strict-flex-arrays -Wno-error=switch-default -Wno-error=switch-enum -Wno-missing-declarations -Wunused -Wno-error=unused-const-variable=2 -Wuse-after-free=3 -Wuninitialized -Wstrict-aliasing=2 -Wfloat-equal -Wpointer-arith -Wundef -Wcast-qual -Wcast-align -Wattributes -Wmissing-format-attribute -Wmultichar -Wpacked -Wno-error=redundant-decls -Wunreachable-code -Winvalid-pch -Wvolatile-register-var -Wdisabled-optimization -Wno-unknown-pragmas -Wwrite-strings -Wstrict-overflow=2 -Wno-cpp -Wno-error=stringop-overflow=2 -Wno-error=array-bounds")
set(C_FIRST_PARTY_DIAGNOSTIC_FLAGS "${C_AND_CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS} -Wbad-function-cast -Wc++-compat -Wnested-externs -Wno-missing-prototypes -Wold-style-definition -Wstrict-prototypes")
set(CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS "${C_AND_CXX_FIRST_PARTY_DIAGNOSTIC_FLAGS} -Wc++20-compat -Wno-error=abi-tag -Waligned-new=all -Wcomma-subscript -Wctad-maybe-unsupported -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wenum-conversion -Winvalid-imported-macros -Wno-error=noexcept -Wredundant-tags -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual=2 -Wsign-promo -Wmismatched-tags -Wuseless-cast -Wvolatile -Wzero-as-null-pointer-constant -Wplacement-new=2 -Wcatch-value=3 -Wconditionally-supported -Wextra-semi -Wno-error=suggest-final-types -Wno-error=suggest-final-methods -Wsuggest-override")

set(CMAKE_C_COMPILER "${COMPILER_BIN_DIR}/${COMPILER_PREFIX}gcc${COMPILER_SUFFIX}")
set(CMAKE_C_STANDARD_COMPUTED_DEFAULT "-std=c18")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -O0 -ggdb3 -fno-eliminate-unused-debug-symbols -fno-eliminate-unused-debug-types -fno-common -fno-omit-frame-pointer -fsanitize=address,undefined -fstack-protector-all -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_SANITIZE_VECTOR")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Ofast -DNDEBUG")
# -fgcse-las -fgcse-sm -fgraphite-identity -finline-limit=5000 -fipa-pta -floop-nest-optimize -ftree-loop-im -ftree-loop-ivcanon -ftree-parallelize-loops=8 -funsafe-loop-optimizations -fvariable-expansion-in-unroller")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -march=tremont -Og -ggdb3 -fno-common -fno-omit-frame-pointer -DNDEBUG")

set(CMAKE_CXX_COMPILER "${COMPILER_BIN_DIR}/${COMPILER_PREFIX}g++${COMPILER_SUFFIX}")
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "-std=c++17")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_CXX_INCLUDE_DIR} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -O0 -ggdb3 -fno-eliminate-unused-debug-symbols -fno-eliminate-unused-debug-types -fno-common -fno-omit-frame-pointer -fsanitize=address,undefined -fstack-protector-all -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_GLIBCXX_SANITIZE_VECTOR")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_CXX_INCLUDE_DIR} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_CXX_INCLUDE_DIR} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -Ofast -DNDEBUG")
# -fgcse-las -fgcse-sm -fgraphite-identity -finline-limit=5000 -fipa-pta -floop-nest-optimize -ftree-loop-im -ftree-loop-ivcanon -ftree-parallelize-loops=8 -funsafe-loop-optimizations -fvariable-expansion-in-unroller")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_STANDARD_COMPUTED_DEFAULT} -isystem ${COMPILER_CXX_INCLUDE_DIR} -isystem ${COMPILER_C_INCLUDE_DIR} -isystem ${OS_INCLUDE_DIR} ${C_AND_CXX_NONDIAGNOSTIC_FLAGS} -march=tremont -Og -ggdb3 -fno-common -fno-omit-frame-pointer -DNDEBUG")

set(LINKER_FLAGS_COMMON "-L${COMPILER_LIB_DIR} -L${COMPILER_LIB64_DIR} -static-libstdc++ -fuse-ld=gold -ltbb")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS_COMMON}")
set(CMAKE_SHARED_LINKER_FLAGS "${LINKER_FLAGS_COMMON}")
set(CMAKE_MODULE_LINKER_FLAGS "${LINKER_FLAGS_COMMON}")

set(CMAKE_FIND_ROOT_PATH)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set_property(GLOBAL PROPERTY CMAKE_DL_LIBS dl)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
