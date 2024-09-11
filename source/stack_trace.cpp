#include "stack_trace.hpp"


#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <cxxabi.h>
#include <libunwind.h>


namespace {
    constexpr std::size_t mangled_buffer_length = 512;
    std::size_t demangled_buffer_length = mangled_buffer_length;
    char mangled_procedure_name_buffer[mangled_buffer_length];
    char * demangled_procedure_name_buffer =
     static_cast<char *>(std::malloc(demangled_buffer_length));
}


void dump_call_stack(FILE * output_fd) {
    unw_context_t context;
    if (-1 == unw_getcontext(&context)) {
        // Cannot obtain call stack information.
        return;
    }
    unw_cursor_t cursor;
    if (0 != unw_init_local(&cursor, &context)) {
        // Cannot obtain call stack information.
        return;
    }

    do {
      unw_word_t stack_pointer;
      if (0 != unw_get_reg(&cursor, UNW_REG_SP, &stack_pointer)) {
          // Cannot obtain further call stack information.
          return;
      }
      unw_word_t instruction_pointer;
      if (0 != unw_get_reg(&cursor, UNW_REG_IP, &instruction_pointer)) {
          // Cannot obtain further call stack information.
          return;
      }
      unw_word_t offset_from_procedure_start;
      if (0 == unw_get_proc_name(&cursor, mangled_procedure_name_buffer,
       mangled_buffer_length, &offset_from_procedure_start)) {
          int result = 0;
          abi::__cxa_demangle(mangled_procedure_name_buffer,
           demangled_procedure_name_buffer, &demangled_buffer_length, &result);
          if (0 != result) {
              // Demangling failed, so use the mangled procedure name.
              std::strncpy(demangled_procedure_name_buffer,
               mangled_procedure_name_buffer, demangled_buffer_length);
          }
      } else {
          std::strncpy(demangled_procedure_name_buffer,
           "unable_to_get_procedure_name", demangled_buffer_length);
          offset_from_procedure_start =
           static_cast<unw_word_t>(0xffffffffffffffffllu);
      }
      std::fprintf(output_fd, "sp = %" PRIx64 ", ip = %" PRIx64
       " = %s + %" PRIx64 "\n",
       static_cast<std::uint64_t>(stack_pointer),
       static_cast<std::uint64_t>(instruction_pointer),
       demangled_procedure_name_buffer,
       offset_from_procedure_start);
      std::fflush(output_fd);
    } while (0 < unw_step(&cursor));
}
