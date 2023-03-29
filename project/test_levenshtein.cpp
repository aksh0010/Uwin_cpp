//=============================================================================

#include <array>
#include <iostream>
#include <string>
#include <vector>
#include "project.hpp"

//=============================================================================

int main()
{
  using namespace std;
  using namespace std::literals;
  using uwindsor_2023w::comp3400::project::levenshtein;

  cout 
    << (levenshtein("kitten", "sitting") == 3) 
    << (levenshtein("Saturday", "Sunday") == 3)
    << (levenshtein("thou shalt not", "you should not") == 5)
    << (levenshtein("","") == 0)
    << (levenshtein(""s,""s) == 0)
    << (levenshtein(""s,vector<char>{}) == 0)
    // NOTE: A C-style string literal implicitly has and includes a '\0'
    //       character at the end of the string which is included in its 
    //       "length" as a literal value. This means the next line returns an
    //       edit distance of 1 instead of zero for an "empty" string.
    << (levenshtein(""s,"") == 1)
    << (levenshtein(string{"house"},"mouse"s) == 1)
    << (levenshtein(vector{'c','a','r'}, array{'b','a','t'}) == 2)
    << (levenshtein(wstring{L"αβδε"}, L"αβ_δε") == 2)
    << (levenshtein(vector{'V','s','a','u','c','e'}, "apple sauce"s) == 6)
    << '\n'
  ;
}

//=============================================================================
