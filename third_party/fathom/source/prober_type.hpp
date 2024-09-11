#if !defined(PROBER_TYPE_HPP)
#define PROBER_TYPE_HPP


#include <array>
#include <optional>
#include <string>

#include "tbprobe.h"

#include "probe_report_type.hpp"
#include "tb_position_type.hpp"


class prober_type {
public:
    static std::optional<prober_type const> create_once_opt(
        std::string const & syzygy_path = "");

    probe_report_type probe(tb_position_type const & tb_position) const;
    probe_report_type probe(std::string const & fen) const;
    probe_report_type probe(
        std::string const & fen,
        tb_position_type & tb_position) const;
    probe_report_type probe(
        std::string const & fen,
        tb_position_type & tb_position,
        unsigned int & tb_probe_root_result,
        std::array<unsigned int, TB_MAX_MOVES> & per_move_root_results) const;

private:
    prober_type() {}
};


#endif // PROBER_TYPE_HPP
