#include "utils.hpp"

#include <vector>
#include <numeric>
#include <random>
#include <functional>
#include <string>
#include <ranges>
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <array>
#include <iostream>

#include <iomanip>

#include <random>

#include <iterator>

namespace uwindsor_2023w
{
    namespace comp3400
    {

        namespace project
        {

            template <typename T>
            constexpr T const &min(T const &a)
            { // i.e., returns a
                return a;
            }

            template <typename T>
            constexpr T const &min(T const &a, T const &b)
            // i.e., like std::min()
            {
                return a < b ? a : b;
            }

            // definition of min() with three or more arguments...
            template <typename T, typename... Rest>
                requires(std::same_as<T, Rest> && ...) // i.e., a C++17 fold expression requiring all Rest... types to be the same as T
            constexpr T const &min(T const &a, T const &b, Rest const &...rest)
            {

                return min(min(a, b), rest...);
            }

            template <typename StringA, typename StringB>
                requires std::ranges::sized_range<StringA> &&
                         std::ranges::sized_range<StringB> &&
                         std::ranges::random_access_range<StringA> &&
                         std::ranges::random_access_range<StringB> &&
                         std::same_as<std::ranges::range_value_t<StringA>, std::ranges::range_value_t<StringB>>
            std::size_t levenshtein(StringA const &a, StringB const &b)
            {
                std::vector<std::size_t> row_vector0(std::ranges::size(b) + 1);
                std::vector<std::size_t> col_vector1(std::ranges::size(b) + 1);

                // std::vector<std::size_t> row_vector0;
                // std::vector<std::size_t> col_vector1;

                // row_vector0.resize(std::ranges::size(a), 0);
                // col_vector1.resize(std::ranges::size(a), 0);
                std::iota(std::ranges::begin(row_vector0), std::ranges::end(row_vector0), 0);
                for (std::size_t i = 0; i < std::ranges::size(a); ++i)
                {
                    col_vector1[0] = i + 1;

                    // auto row_vector0_iter = std::begin(row_vector0);
                    // auto col_vector1_iter = std::begin(col_vector1);
                    //  ++row_vector0_iter, ++col_vector1_iter
                    for (std::size_t j = 0; j < std::ranges::size(b); ++j)
                    {
                        //     std::size_t deletion_cost = *row_vector0_iter + 1;
                        //     std::size_t insertion_cost = *(col_vector1_iter - 1) + 1;
                        //     std::size_t substitution_cost = *row_vector0_iter + (a[i] == b[j] ? 0 : 1);

                        //     *col_vector1_iter = std::min({deletion_cost, insertion_cost, substitution_cost});
                        //
                        std::size_t deletion_cost = row_vector0[j + 1] + 1;
                        std::size_t insertion_cost = col_vector1[j] + 1;
                        std::size_t sub_cost = 0;
                        if (a[i] == b[j])
                        {
                            sub_cost = row_vector0[j];
                        }
                        else
                        {
                            sub_cost = row_vector0[j] + 1;
                        }
                        col_vector1[j + 1] = uwindsor_2023w::comp3400::project::min(deletion_cost, insertion_cost, sub_cost);
                    }

                    std::swap(row_vector0, col_vector1);
                }

                return row_vector0[std::ranges::size(b)];
            }

            class char_mutator
            {
            private:
                std::string valid_chars;
                mutable std::uniform_int_distribution<std::size_t> uniform_distribution;
                mutable std::default_random_engine randomEngine;

            public:
                template <typename... Args>
                char operator()(Args &&...) const
                {
                    std::size_t index = uniform_distribution(randomEngine);
                    return valid_chars[index];
                }

                char_mutator()
                    : valid_chars([]()
                                  {
                                      std::string chars;
                                      for (short i = 0; i <= std::numeric_limits<char>::max(); ++i)
                                      {
                                          if (std::isalnum(i) || std::ispunct(i) || i == ' ')
                                          {
                                              chars.push_back(i);
                                          }
                                      }
                                      return chars; }()),
                      uniform_distribution(0, valid_chars.size()), randomEngine(std::random_device{}()){};
            };

            template <
                std::ranges::range Individual,
                typename MutateOp,
                typename URBG>
                requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>> &&
                         std::invocable<MutateOp, std::ranges::range_value_t<Individual>>
            void mutate(
                Individual &individual,
                double const rate,
                MutateOp &&m,
                URBG &&urbg)
            {
                std::uniform_real_distribution<double> dist(0.0, 1.0);
                auto lambda = [&](auto &arg)
                {
                    if (dist(urbg) < rate)
                    {
                        arg = m(arg);
                    }
                };
                std::ranges::for_each(individual, lambda);
            }

            template <typename URBG1, typename URBG2, typename Individual>
                requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG1>> &&
                         std::uniform_random_bit_generator<std::remove_cvref_t<URBG2>> &&
                         std::ranges::forward_range<Individual> &&
                         std::ranges::sized_range<Individual> &&
                         smart_insertable<Individual>
                         auto crossover(
                             std::size_t const ncrossover_points,
                             URBG1 &&urbg_starting_parent,
                             URBG2 &&urbg_crossover_points,
                             Individual const &parent1,
                             Individual const &parent2) -> std::remove_cvref_t<Individual>
            {
                // Determine the minimum size of the two parents.
                auto psize_truncated = std::min(std::ranges::size(parent1), std::ranges::size(parent2));

                // Declare a bernoulli_distribution variable with probability 0.5.
                std::bernoulli_distribution which_parent_dist(0.5);

                // Identify which parent is currently being copied from.
                auto which_parent = which_parent_dist(urbg_starting_parent);

                // If any of ncrossover_points or parent1's range size or parent2's range size are 0, return the parent argument as-is per the which_parent variable's value.
                if (ncrossover_points == 0 || psize_truncated == 0)
                {
                    return which_parent ? parent1 : parent2;
                }

                // Declare a vector to hold the indices of each crossover point.
                std::vector<size_t> crossover_indices(psize_truncated - 1);

                // Fill the vector with indices starting at 1.
                std::iota(crossover_indices.begin(), crossover_indices.end(), 1);

                // Declare a vector to hold the crossover offset values.
                std::vector<size_t> crossover_offsets;

                // Reserve space for crossover_offsets to avoid reallocations.
                reserve_or_noop(crossover_offsets, ncrossover_points);

                // Sample ncrossover_points indices without replacement from crossover_indices.
                std::ranges::sample(crossover_indices, std::back_inserter(crossover_offsets), ncrossover_points, urbg_crossover_points);

                // Compute the adjacent differences of crossover_offsets to get the relative
                // std::adjacent_difference(crossover_offsets.begin(), crossover_offsets.end());
                std::back_insert_iterator<std::vector<std::size_t>> back_it(crossover_offsets);
                std::sort(crossover_offsets.begin(), crossover_offsets.end());

                which_parent = urbg_starting_parent();
                auto child = Individual{};
                child.reserve(psize_truncated);

                auto last_offset = std::size_t{0};
                for (auto const offset : crossover_offsets)
                {
                    auto const chunk_size = offset - last_offset;
                    if (which_parent)
                    {
                        std::ranges::copy_n(std::ranges::begin(parent1) + last_offset, chunk_size, std::back_inserter(child));
                    }
                    else
                    {
                        std::ranges::copy_n(std::ranges::begin(parent2) + last_offset, chunk_size, std::back_inserter(child));
                    }
                    which_parent ^= true;
                    last_offset = offset;
                }

                auto const last_chunk_size = psize_truncated - last_offset;
                if (which_parent)
                {
                    std::ranges::copy_n(std::ranges::begin(parent1) + last_offset, last_chunk_size, std::back_inserter(child));
                }
                else
                {
                    std::ranges::copy_n(std::ranges::begin(parent2) + last_offset, last_chunk_size, std::back_inserter(child));
                }

                return child;
            }

        }
    }

}
