#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "cube_binary_inputs.hpp"
#include "load_and_save.hpp"
#include "parallel_for_each.hpp"


using decision_dag_type = ddag<32, 8, 27>;


class ddag_test_stub_type {
public:
    ddag_test_stub_type & operator=(ddag_test_stub_type &&) = default;
    ddag_test_stub_type & operator=(ddag_test_stub_type const &)
     = delete;
    ddag_test_stub_type(ddag_test_stub_type &&) = default;
    ddag_test_stub_type(ddag_test_stub_type const &) = delete;
    ~ddag_test_stub_type() = default;
    ddag_test_stub_type(decision_dag_type && tree)
        : the_endgame_data(tree) {}

    std::size_t byte_count() {
        return the_endgame_data.nbytes();
    }
    std::size_t node_count() {
        return the_endgame_data.nnodes();
    }

    unsigned char lookup(std::uint64_t const value) {
        cube_binary_inputs_tt<32> const direct_cube{
         cube_binary_inputs_tt<32>(value)};
        std::cerr << "II: direct_cube = " << direct_cube << '\n';
        unsigned char const lookup_result{the_endgame_data.lookup(direct_cube)};
        std::cerr << "II: lookup_result = "
         << static_cast<int>(lookup_result) << " ("
         << to_binary_string(lookup_result, 3) << ").\n";
        return lookup_result;
    }

private:
    decision_dag_type the_endgame_data;
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv) {
    std::string_view const material_balance("KP_vs_k");
    std::optional<decision_dag_type> ddag_opt(
     load_ddag_opt<32, 8, 27>(material_balance));
    if (not ddag_opt) {
        std::cerr << "EE: Unable to load ddag (" << material_balance << ")\n";
        std::exit(EXIT_FAILURE);
    }
    std::cerr << "II: *ddag_opt size (bytes) = " << ddag_opt->nbytes()
     << "\nII: *ddag_opt size (nodes) = " << ddag_opt->nnodes() << '\n';
    std::unique_ptr<ddag_test_stub_type> ddag_ptr{
     std::make_unique<ddag_test_stub_type>(std::move(*ddag_opt))};
    std::cerr << "II: *ddag_ptr size (bytes) = " << ddag_ptr->byte_count()
     << "\nII: *ddag_ptr size (nodes) = " << ddag_ptr->node_count() << '\n';

    std::uint64_t q2 = 0b01010110011010010110100110010110101001;
    std::cerr << "II: init value for direct_cube = " << q2 << '\n';
    cube_binary_inputs_tt<32> const direct_cube{cube_binary_inputs_tt<32>(q2)};
    std::cerr << "II: direct_cube = " << direct_cube << '\n';
    unsigned char treeres3{ddag_ptr->lookup(q2)};
    std::cerr << "II: lookup(q2) = " << static_cast<int>(treeres3) << " ("
     << to_binary_string(treeres3, 3) << ")." << '\n';
    return EXIT_SUCCESS;
}
