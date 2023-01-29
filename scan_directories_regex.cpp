/**
 * @file scan_directories_regex.cpp
 * @author Aksh Patel (University of Windsor: Sid. : 110028941)
 * @brief recursively scan directories using regex
 * @version 0.1
 * @date 2023-01-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <regex>
#include <filesystem>
#include <iomanip>
#include <string>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char **argv)
{

    if (argc != 2 && argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <path> [<regex>]\n";
        return 1;
    }
    else
    {
        char const *re = ".*";

        if (argc == 3)
        {
            re = argv[2];
        }

        regex compiled_re(re);
        fs::path const base = argv[1];
        cout << argv[1] << " is th argv\n";


        for (auto const &entry : fs::recursive_directory_iterator(base, fs::directory_options::skip_permission_denied))
        {
            smatch m;
            if (regex_match(entry.path().filename().native(), m, compiled_re))
            {
                string const native_entry = entry.path().lexically_relative(base).native();
                // string native_entry = entry.path().native();
                auto const p = quoted(native_entry);

                if (fs::is_symlink(entry))
                {
                    cout << "LINK: " << p << '\n';
                }
                else if (fs::is_regular_file(entry))
                {
                    cout << "FILE: " << p << '\n';
                }
                else if (fs::is_directory(entry))
                {

                    cout << "DIR: " << p << '\n';
                }
                else
                {
                   
                    cout << "OTHER: " << p << '\n';
                }
            }
        }
    }
}
