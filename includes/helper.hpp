// -*- C++ -*-
//===------------------------- commander.hpp ------------------------------===//
//
//  Part of dotfiles management tool
//
//===----------------------------------------------------------------------===//

#ifndef CLI_HELPER_HPP
#define CLI_HELPER_HPP

#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <cctype>
#include <locale>

namespace cli
{
    namespace helper
    {
        std::vector<std::string> tokenize(const std::string str, const std::regex re);
        static inline void ltrim(std::string &s);
        static inline void rtrim(std::string &s);
        static inline void trim(std::string &s);

        std::vector<std::string> tokenize(const std::string str, const std::regex re)
        {
            std::sregex_token_iterator it{str.begin(), str.end(), re, -1};
            std::vector<std::string> tokenized{it, {}};
            // remove empty strings
            tokenized.erase(std::remove_if(tokenized.begin(), tokenized.end(),
                                           [](std::string const &s) { return s.size() == 0; }),
                            tokenized.end());

            return tokenized;
        }

    } // namespace helper

} // namespace cli

#endif