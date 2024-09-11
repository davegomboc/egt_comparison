#include "egt_format_type.hpp"


#include <array>
#include <memory>


egt_format_type *
egt_format_type::create_ptr(std::string_view const egt_format_sv) {
    std::array<std::unique_ptr<egt_format_type>, 9> const supported_formats{
     std::make_unique<syzygy_wcdbl_egt_format_type>(),
     std::make_unique<syzygy_dtzm_egt_format_type>(),
     std::make_unique<chesspresso_binary_egt_format_type>(),
     std::make_unique<cube_list_egt_format_type>(),
     std::make_unique<indexed_cube_list_egt_format_type>(),
     std::make_unique<two_level_logic_minimization_egt_format_type>(),
     std::make_unique<decision_dag_egt_format_type>(),
     std::make_unique<probe_report_nybble_egt_format_type>(),
     std::make_unique<multiterminal_binary_decision_diagram_egt_format_type>()};

    if ("clst" == egt_format_sv) {
        return new cube_list_egt_format_type();
    }
    if ("dtr" == egt_format_sv) {
        return new decision_dag_egt_format_type();
    }
    if ("icl" == egt_format_sv) {
        return new indexed_cube_list_egt_format_type();
    }
    if ("mtbdd" == egt_format_sv) {
        return new multiterminal_binary_decision_diagram_egt_format_type();
    }
    if ("plb" == egt_format_sv) {
        return new chesspresso_binary_egt_format_type();
    }
    if ("rtbw" == egt_format_sv) {
        return new syzygy_wcdbl_egt_format_type();
    }
    if ("rtbz" == egt_format_sv) {
        return new syzygy_dtzm_egt_format_type();
    }
    if ("tllm" == egt_format_sv) {
        return new two_level_logic_minimization_egt_format_type();
    }
    if ("wpr" == egt_format_sv) {
        return new probe_report_nybble_egt_format_type();
    }
    return nullptr;
}


// These primarily exists so that clang++ will emit vtables for these classes in
// this translation unit only, instead of all of them.  (See -Wweak-vtables
// documentation for details.)
void egt_format_type::vtable_pinning_method() const {}
void syzygy_wcdbl_egt_format_type::vtable_pinning_method() const {}
void syzygy_dtzm_egt_format_type::vtable_pinning_method() const {}
void chesspresso_binary_egt_format_type::vtable_pinning_method() const {}
void cube_list_egt_format_type::vtable_pinning_method() const {}
void indexed_cube_list_egt_format_type::vtable_pinning_method() const {}
void two_level_logic_minimization_egt_format_type::vtable_pinning_method() const {}
void decision_dag_egt_format_type::vtable_pinning_method() const {}
void probe_report_nybble_egt_format_type::vtable_pinning_method() const {}
void multiterminal_binary_decision_diagram_egt_format_type
 ::vtable_pinning_method() const {}


namespace {
chesspresso_binary_egt_format_type chesspresso;
cube_list_egt_format_type clst;
indexed_cube_list_egt_format_type icl;
two_level_logic_minimization_egt_format_type tllm;
decision_dag_egt_format_type ddag;
probe_report_nybble_egt_format_type wpr;
multiterminal_binary_decision_diagram_egt_format_type mtbdd;
syzygy_wcdbl_egt_format_type rtbw;
syzygy_dtzm_egt_format_type rtbz;
}
