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
#include <exception.hpp>
#include <locale>

namespace cli
{

namespace helper
{

/**
 * @brief Tokenize the given vector according to the regex and remove the empty
 * tokens.
 * 
 * @param str 
 * @param re 
 * @return std::vector<std::string> 
 */
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

/**
 * @brief process_args, and returns the pair with processed arg and readable 
 * property.
 * 
 * @param argument 
 * @return std::pair<std::string, int> 
 */
std::pair<std::string, int> process_arg(const std::string & argument)
{
    if (argument.length() < 3) throw Exception(errstr::option::INVALID_ARG + argument);
    // process the arguments 
    std::string arg_name = argument.substr(1, argument.length() - 2);

    return {arg_name, (argument.front() == '<') ? 1 : 0};
}

} // namespace helper

} // namespace cli

#endif