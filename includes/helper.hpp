// -*- C++ -*-
//===---------------------------- helper.hpp ------------------------------===//
//  
//  Copyright (c) 2020 Manish sahani
// 
//  This program is free software: Licensed under the MIT License. you may not 
//  use this file except in compliance with the License. You may obtain a copy 
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
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