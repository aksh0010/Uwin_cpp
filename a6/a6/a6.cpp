#include <algorithm>  // e.g., std::for_each, std::sort, std::find
#include <cstddef>    // e.g., std::size_t
#include <functional> // e.g., std::greater
#include <fstream>    // e.g., std::ifstream
#include <iterator>   // e.g., std::iterator_traits, std::forward_iterator
#include <iostream>   // e.g., std::cout, std::clog
#include <map>        // e.g., std::map
#include <set>        // e.g., std::set
#include <string>     // e.g., std::string
#include <vector>     // e.g., std::vector

template <std::forward_iterator FwdIter>
std::ostream &output(
    std::ostream &os,
    FwdIter first,
    FwdIter last,
    bool const with_indices = false)
{
    using count_type = typename std::iterator_traits<FwdIter>::difference_type;

    count_type index = 0;
    count_type rindex = std::distance(first, last);

    std::for_each(first, last, [&](auto const &elem)
                  {
    os << elem;
    if (with_indices) {
      os << " (" << index << ":" << -rindex << ")";
    }
    os << " ";
    ++index;
    --rindex; });

    return os;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <text_file_path>\n";
        return 1;
    }

    std::ifstream file;
    file.open(argv[1], std::ios::in);

    std::map<std::string, std::size_t> freq_hist;
    std::vector<std::string> words;

    if (file.is_open())
    {
        std::string temp;
        while (file >> temp)
        {
            words.push_back(temp);
            freq_hist[temp]++;
        }
        file.close();
    }
    else
    {

        std::cerr << "Error opening the file \n";
        return 1;
    }

    if (words.empty())
    {
        std::cerr << "There is no data. Aborting.\n";
        return 2;
    }

    // Reversing The Words
    std::vector<std::string> revwords;
    std::reverse_copy(words.begin(), words.end(), std::back_inserter(revwords));

    std::map<std::size_t, std::set<std::string>> rev_freq_hist;
    // Computing the Inverse Frequency Histogram
    auto i = freq_hist.begin();
    auto i_end = freq_hist.end();

    for (; i != i_end; i++)
    {

        rev_freq_hist.insert(std::make_pair(i->second, std::set<std::string>())).first->second.insert(i->first);
    }

    // Determining the Smallest Number of Words Having the Largest Frequency
    auto smallest_entry = std::min_element(rev_freq_hist.begin(), rev_freq_hist.end(),
                                           [](auto const &lhs, auto const &rhs)
                                           { return lhs.second.size() < rhs.second.size(); });

    std::cout << "frequency " << smallest_entry->first << " occurs with these strings: ";
    output(std::cout, smallest_entry->second.begin(), smallest_entry->second.end(), true);
    std::cout << std::endl;

    auto max_it = std::max_element(rev_freq_hist.begin(), rev_freq_hist.end(),
                                   [](auto const &lhs, auto const &rhs)
                                   {
                                       return lhs.second.size() < rhs.second.size();
                                   });

    // Finding the First Position "lo" Appears in revwords + an Adjustment
    std::string lo = *(max_it->second.begin());
    std::string hi = *(std::prev(max_it->second.end()));

    std::cout << "lo value: " << lo << std::endl;
    std::cout << "hi value: " << hi << std::endl;

    auto lopos = std::find(revwords.begin(), revwords.end(), lo);
    if (lopos != revwords.end())
    {
        if (lopos != std::prev(revwords.end()))
        {
            ++lopos;
        }
        auto lo_end = std::find_if_not(lopos, revwords.end(), [&](auto const &elem)
                                       { return elem == lo; });
        auto lo_index = std::distance(revwords.begin(), lopos);
        std::cout << "lo index: " << lo_index << std::endl;
    }
    else
    {
        std::cout << "lo not found in revwords" << std::endl;
    }
    // Finding the Last Occurrence of "hi" in revwords + an Adjustment
    auto rhipos = std::find_end(revwords.rbegin(), std::reverse_iterator(lopos), &hi, &hi + 1);
    if (rhipos != std::reverse_iterator(lopos))
    {
        ++rhipos;
    }
    auto hi_index = std::distance(revwords.begin(), rhipos.base()) - 1;

    std::sort(lopos, rhipos.base(), [](const std::string &a, const std::string &b)
              { return std::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end()); });
    // Output all words reversed
    std::cout << "All words reversed: ";
    output(std::cout, words.rbegin(), words.rend(), true);
    std::cout << std::endl;

    // Sorting in Descending Order
    std::sort(lopos, rhipos.base(), [](const std::string &a, const std::string &b)
              { return a > b; });

    // Producing the Final Outputs
    std::cout << "All words reversed with [" << hi_index << "," << hi_index << ") sorted: ";
    output(std::cout, revwords.begin(), revwords.end(), false);
    std::cout << std::endl;

    return 0;
}
