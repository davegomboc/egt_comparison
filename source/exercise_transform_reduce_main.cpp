#include <execution>
#include <iostream>
#include <numeric>
#include <random>


#include "execution_measurement.hpp"


struct ordered_item_type {
    int quantity;
    double regular_per_unit_price;

    double per_unit_price_in_quantity() const {
        if (100 <= quantity) {
            return 0.8 * regular_per_unit_price;
        } else if (10 <= quantity) {
            return 0.9 * regular_per_unit_price;
        } else {
            return regular_per_unit_price;
        }
    }

    static ordered_item_type generate() {
        ordered_item_type result;
        result.quantity = the_quantity_distribution(
         the_pseudorandom_number_generator);
        result.regular_per_unit_price = the_regular_price_distribution(
         the_pseudorandom_number_generator);
        return result;
    }

private:
    static std::mt19937_64 the_pseudorandom_number_generator;
    static std::uniform_int_distribution<unsigned char>
     the_quantity_distribution;
    static std::uniform_real_distribution<double>
     the_regular_price_distribution;
};

std::mt19937_64 ordered_item_type::the_pseudorandom_number_generator{};
std::uniform_int_distribution<unsigned char>
 ordered_item_type::the_quantity_distribution{};
std::uniform_real_distribution<double>
 ordered_item_type::the_regular_price_distribution{};


// Verify that C++ can compile and invoke std::transform_reduce.
template <typename iterator_type, typename execution_policy_type>
double price_of_order(
    iterator_type first,
    iterator_type past_last,
    execution_policy_type && execution_policy
) {
    return std::transform_reduce(
     std::forward<execution_policy_type>(execution_policy), first, past_last,
     0.0, std::plus<>(),
     [](ordered_item_type const & ordered_item) {
         return ordered_item.quantity
          * ordered_item.per_unit_price_in_quantity();
     });
}


std::vector<ordered_item_type> const generate_order(std::size_t const limit) {
    std::vector<ordered_item_type> result;
    for (std::size_t i{}; i != limit; ++i) {
        result.push_back(ordered_item_type::generate());
    }
    return result;
}

void benchmark(std::vector<ordered_item_type> const & order) {
    double const total{price_of_order(order.cbegin(), order.cend(),
     std::execution::seq)};
    std::cout << total << std::endl;
}


int main(void) {
    std::vector<ordered_item_type> const order(generate_order(10'000'000));
    double total_seq;
    auto const & [start_seq, stop_seq] = measure_execution_time([&]() {
        total_seq = price_of_order(order.cbegin(), order.cend(),
         std::execution::seq);
    });
    auto const duration_seq{stop_seq - start_seq};

    double total_par;
    auto const & [start_par, stop_par] = measure_execution_time([&]() {
        total_par = price_of_order(order.cbegin(), order.cend(),
         std::execution::par);
    });
    auto const duration_par{stop_par - start_par};

    double total_par_unseq;
    auto const & [start_par_unseq, stop_par_unseq]
     = measure_execution_time([&]() {
        total_par_unseq = price_of_order(order.cbegin(), order.cend(),
         std::execution::par_unseq);
    });
    auto const duration_par_unseq{stop_par_unseq - start_par_unseq};

    std::cout << "total_seq = " << total_seq
     << "\nduration_seq = " << duration_seq.count()
     << "\ntotal_par = " << total_par
     << "\nduration_par = " << duration_par.count()
     << "\ntotal_par_unseq = " << total_par_unseq
     << "\nduration_par_unseq = " << duration_par_unseq.count() << std::endl;
}
