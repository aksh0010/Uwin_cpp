//=============================================================================

#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include "project.hpp"

//=============================================================================

int main()
{
  using namespace std;
  using uwindsor_2023w::comp3400::project::min;
  using uwindsor_2023w::comp3400::project::crossover;

  string const& parent1{ "________________________________________" };
  string const& parent2{ "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" };
  cout 
    << "p1:\t" << quoted(parent1) << '\n'
    << "p2:\t" << quoted(parent2) << "\n\n"
  ;

  random_device rd;

  // Declare random engines for crossover()...
  default_random_engine which_parent_re{rd()};
  default_random_engine crossover_points_re{rd()};
  
  for (int i{}; i != 4; ++i)
  {
    for (int repeat{}; repeat != 8; ++repeat)
      cout 
        << i << ":\t" 
        << quoted(crossover(i, which_parent_re, crossover_points_re, 
            parent1, parent2)) 
        << '\n'
      ;
    cout << '\n';
  }

  // set up random engine and distribution for the number of crossover
  // points...
  default_random_engine num_crossovers_re{rd()};
  uniform_int_distribution<size_t> num_crossovers_dist(
    0, min(parent1.size(), parent2.size())
  );

  for (int i{}; i != 20; ++i)
  {
    auto const n = num_crossovers_dist(num_crossovers_re);
    cout 
      << n << ":\t" 
      << quoted(crossover(n, which_parent_re, crossover_points_re, 
          parent1, parent2)) 
      << '\n'
    ;
  }
}

//=============================================================================
