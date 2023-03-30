#include "utils.hpp"
// Including required gaurds for the header to run
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
// namespace uwindsor_2023w has --> namespace comp3400 has ---> namespace project which has --> all min functions, class char_mutator, function levenshtein, function mutate and function crossover
namespace uwindsor_2023w
{
    namespace comp3400
    {

        namespace project
        {

            template <typename T>
            constexpr T const &min(T const &a)
            {
                // simply returing the only element sent as a param
                return a;
            }

            template <typename T>
            constexpr T const &min(T const &a, T const &b)

            {
                // using ternary operator to return the minimum of both the values
                return a < b ? a : b;
            }

            // definition of min() with three or more arguments...
            template <typename T, typename... Rest>
                requires(std::same_as<T, Rest> && ...)
            constexpr T const &min(T const &a, T const &b, Rest const &...rest)
            {
                // using recurssion to call min function depending on the params which are greator than equal to 3
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

                // Creating two vectors both with equal size.
                //  one vector used for row and other used for column

                std::vector<std::size_t> row_vector0(std::ranges::size(b) + 1);
                std::vector<std::size_t> col_vector1(std::ranges::size(b) + 1);

                // using std::iota to initilize the vector to 0,1,2,3... till the size of vector row

                std::iota(std::ranges::begin(row_vector0), std::ranges::end(row_vector0), 0);
                for (std::size_t i = 0; i < std::ranges::size(a); ++i)
                {
                    col_vector1[0] = i + 1;
                    for (std::size_t j = 0; j < std::ranges::size(b); ++j)
                    {
                        std::size_t deletion_cost = row_vector0[j + 1] + 1; // calculating the deletion cost of an element in the vector
                        std::size_t insertion_cost = col_vector1[j] + 1;    // calculating the insertion cost on and element
                        std::size_t sub_cost = 0;                           // initilizing the substitution cost var to 0 and then computing it as per conditions
                        if (a[i] == b[j])
                        {
                            sub_cost = row_vector0[j];
                        }
                        else
                        {
                            sub_cost = row_vector0[j] + 1;
                        }
                        // calling the min function from our namespace to get the minimum of Deletion, insertion and substitution cost
                        col_vector1[j + 1] = uwindsor_2023w::comp3400::project::min(deletion_cost, insertion_cost, sub_cost);
                    }
                    // simply swaping the row and col vectors
                    std::swap(row_vector0, col_vector1);
                }
                // returing the right bottom most cell that is token edit score.
                return row_vector0[std::ranges::size(b)];
            }
            // char_mutator class below
            class char_mutator
            {

                // Declaring required private variables

            private:
                std::string valid_chars;
                mutable std::uniform_int_distribution<std::size_t> uniform_distribution;
                mutable std::default_random_engine randomEngine;

                // Declaring one public operator overload for ()
            public:
                template <typename... Args>
                char operator()(Args &&...) const
                {
                    std::size_t index = uniform_distribution(randomEngine);
                    return valid_chars[index];
                }
                // Declaring one default constructor initilizing all the elements in the member initilization list
                // The first private member is initilized using the lambda function inside which we iterate using short int till std::numeric_limits<char>::max()

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
            // Mutate function below
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
                std::uniform_real_distribution<double> dist(0.0, 1.0); // Declaring the dist element in range 0-1 using the constructor call
                auto lambda = [&](auto &arg)
                {
                    // lambda function for calcualting arg if urbg's distribution is less than rate
                    if (dist(urbg) < rate)
                    {
                        arg = m(arg);
                    }
                };
                std::ranges::for_each(individual, lambda); // for_each on individual (param var) , and lambda (which we just created)
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
                auto psize_truncated = uwindsor_2023w::comp3400::project::min(std::ranges::size(parent1), std::ranges::size(parent2));

                // Declaring a bernoulli_distribution variable with probability 0.5.
                std::bernoulli_distribution which_parent_dist(0.5);

                // Identify which parent is currently being copied from.
                auto which_parent = which_parent_dist(urbg_starting_parent);

                if (ncrossover_points == 0 || psize_truncated == 0)
                {
                    // If any of ncrossover_points or parent1's range size or parent2's range size are 0, return the parent argument as-is per the which_parent variable's value.
                    return which_parent ? parent1 : parent2;
                }

                // Declare a vector to hold the indices of each crossover point.
                std::vector<size_t> crossover_indices(psize_truncated - 1);

                // Fill the vector with indices starting at 1.
                std::iota(crossover_indices.begin(), crossover_indices.end(), 1);

                // Declare a vector to hold the crossover offset values.
                std::vector<size_t> crossover_offsets(psize_truncated - 1);

                // Reserve space for crossover_offsets to avoid reallocations this is from utils.hpp
                reserve_or_noop(crossover_offsets, ncrossover_points);

                // Sample ncrossover_points indices without replacement from crossover_indices.
                std::ranges::sample(crossover_indices, std::back_inserter(crossover_offsets), ncrossover_points, urbg_crossover_points);

                // Compute the adjacent differences of crossover_offsets to get the relative
                std::adjacent_difference(crossover_offsets.begin(), crossover_offsets.end(), crossover_offsets.begin());

                auto retval = Individual{};                                                                // creating the retval of type individual to store results and send them at the end.
                reserve_or_noop(retval, std::max(std::ranges::size(parent1), std::ranges::size(parent2))); // Reserve space for crossover_offsets to avoid reallocations this is from utils.hpp
                auto out = smart_inserter(retval);                                                         // Calling smart_inserter passing retval storing the return value in a variable smart_inserter from utils.hpp
                auto p1pos = std::ranges::cbegin(parent1);                                                 // constant iterator pointing to begining of parent1
                auto p2pos = std::ranges::cbegin(parent2);                                                 // constant iterator pointing to begining of parent2
                for (auto offset : crossover_offsets)
                {
                    if (which_parent) // parent1 block
                    {                 // copying the content to out
                        out = std::copy_n(p1pos, offset, out);
                    }
                    else // parent2 block
                    {
                        // copying the content to out
                        out = std::copy_n(p2pos, offset, out);
                    }
                    std::advance(p1pos, offset); // adjusting both iterators by offset
                    std::advance(p2pos, offset);
                    which_parent = !which_parent;
                }

                if (which_parent) // parent1 block
                {
                    std::ranges::copy(p1pos, std::ranges::cend(parent1), out);
                }
                else // parent2 block
                {
                    std::ranges::copy(p2pos, std::ranges::cend(parent2), out);
                }
                return retval;
            }

        }
    }

}
