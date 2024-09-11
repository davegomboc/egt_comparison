#include "endgame_tables_type_test.hpp"


#include "representation/material_signature_type.hpp"

#include "egt_format_type.hpp"


using representation::material_signature_type;


TEST(EndgameTablesType, InitializeMtbdd) {
    multiterminal_binary_decision_diagram_egt_format_type egt_format;
    endgame_tables_type egts(3, egt_format, true);
}

TEST(EndgameTablesType, InitializeChesspressoBinary) {
    chesspresso_binary_egt_format_type egt_format;
    endgame_tables_type egts(3, egt_format, true);
}

TEST(EndgameTablesType, InitializeCubeList) {
    cube_list_egt_format_type egt_format;
    endgame_tables_type egts(3, egt_format, true);
}

TEST(EndgameTablesType, InitializeDecisionDag) {
    decision_dag_egt_format_type egt_format;
    endgame_tables_type egts(3, egt_format, true);
}

TEST(EndgameTablesType, InitializeIndexedCubeList) {
    indexed_cube_list_egt_format_type egt_format;
    endgame_tables_type egts(3, egt_format, true);
}

TEST(EndgameTablesType, InitialProbeReportNybble) {
    probe_report_nybble_egt_format_type egt_format;
    endgame_tables_type egts(3, egt_format, true);
}
