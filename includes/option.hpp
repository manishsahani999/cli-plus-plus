// -*- C++ -*-
//===---------------------------- option.hpp ------------------------------===//
//  
//  Copyright (c) 2020 Manish sahani
// 
//  This program is free software: Licensed under the MIT License. you may not 
//  use this file except in compliance with the License. You may obtain a copy 
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//===----------------------------------------------------------------------===//

#ifndef CLI_LIB_OPTION_HPP
#define CLI_LIB_OPTION_HPP

#include <string>
#include <map>
#include <ostream>
#include <helper.hpp>
#include <colors.hpp>
#include <utility>

namespace cli
{

class Option
{
    /**
     * @brief number of required arguments for an option at the runtime, agreed
     * by the syntax '<>' is required '[]' is optional.
     * 
     */
    int required = 0;
    
    /**
     * @brief maximun arguments allowed for an option at the runtime, this will
     * be not same as required if '[]' is present in the option building.
     * 
     */
    int maxargs = 0;

    /**
     * @brief number of arguments are provided at the runtime, also used for va
     * -lidation.
     * 
     */
    int present = 0;

    /**
     * @brief Usuage string for the option, same as the string provided at the 
     * runtime.
     * 
     */
    std::string usuage;

    /**
     * @brief primary identifier for the option, it can be both, but if small f
     * -lag (ex - '-d') is provided it will be given priority over larger  flag 
     * with.
     * 
     */
    std::string flag;

    /**
     * @brief secondary identifer for the option if small single character flag
     * is present the other larger flag will be considered as secondary flag.  
     *  
     */
    std::string secondary_flag;

    /**
     * @brief description of the option that will be printed in the output of u
     * -sage.
     * 
     */
    std::string description;

    /**
     * @brief Vector of Option args names, and their values 
     * 
     */
    std::vector<std::pair<std::string, std::string>> args;

public:
    Option(const std::string & flag, const std::string & description = "");

    //===-----------------------------------------------------------------===//
    //                                                                       //
    //  Options's api for validating and updating at runtime                 //
    //                                                                       //
    //===-----------------------------------------------------------------===//

    void parse(std::vector<std::string> & args);

    /**
     * @brief Get a copy of option's arguments vector. 
     *
     * @return std::vector<std::pair<std::string, std::string>> 
     */
    std::vector<std::pair<std::string, std::string>> get_argv() const;
    
    /**
     * @brief Overload the == operator, to check with string 
     * 
     * @param f 
     * @return true 
     * @return false 
     */
    bool operator==(const std::string & f) const noexcept;

    /**
     * @brief Overload the == operator according to Option object
     * 
     * @param o 
     * @return true 
     * @return false 
     */
    bool operator==(const Option & o) const noexcept;

    /**
     * @brief Overload the << operator, used for usage listing 
     * 
     * @param os 
     * @param o 
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream & os, const Option & o);
};

/**
 * @brief Construct a new Option:: Option object
 * 
 * @param flag 
 * @param description 
 */
Option::Option(const std::string & flag, const std::string & description)
{
    this->description = description;
    this->usuage = std::regex_replace(flag, std::regex(R"([|]+)"), ", ");
    // process the flag type. if the flag has arguments, then update the requir
    // -ed according to <> or [] provided
    const std::vector<std::string> tokenized = helper::tokenize(flag, 
                                                     std::regex(R"([\s|,]+)"));
    int idx = 0;
    // check the syntax of the option and build according to it Update the prim
    // -ary identifer (flag) of this option
    if (tokenized.size() == 0 || (tokenized.size() && 
                                  tokenized[idx].front() != '-')) 
        throw Exception(errstr::option::INVALID_SYNTAX);

    this->flag = tokenized[idx++];

    // check the tokens for secondary flag, if present then update the option
    if (tokenized.size() >= 2 && tokenized[idx].front() == '-')
        this->secondary_flag = tokenized[idx++];

    // check for the aliases syntax, if more then one aliases are present throw
    if (this->secondary_flag.length() && tokenized.size() >= 3 && 
                                         tokenized[2].front() == '-') 
        throw Exception(errstr::option::INVALID_ALIASES);
    
    // Update the args from the syntax
    for (; idx < tokenized.size(); idx++) 
    {
        auto arg = helper::process_arg(tokenized[idx]);
        
        this->args.push_back({arg.first, ""});
        this->required += arg.second, this->maxargs++;
    }
}

void Option::parse(std::vector<std::string> & args)
{
    if (args.size() < this->required) throw Exception(errstr::option::ARG_MISSING);
    
    for(int i = 0; i < args.size() && i < this->maxargs; i++) 
        this->args[i].second = args[i];
}

/**
 * @brief Get a copy of option's arguments vector. 
 * 
 * @return std::vector<std::pair<std::string, std::string>> 
 */
std::vector<std::pair<std::string, std::string>> Option::get_argv() const
{
    return this->args;
}

/**
 * @brief Overload the == operator to check with string 
 * 
 * @param f 
 * @return true 
 * @return false 
 */
bool Option::operator==(const std::string & f) const noexcept
{
    return (f == this->flag || f == this->secondary_flag) ? true : false;
}

/**
 * @brief Overload the == operator according to Option object
 * 
 * @param o 
 * @return true 
 * @return false 
 */
bool Option::operator==(const Option & o) const noexcept
{
    return (o.flag == this->flag || o.secondary_flag == this->secondary_flag);
}

/**
 * @brief Overload the << operator, used for usage listing 
 * 
 * @param os 
 * @param o 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream & os, const Option & o)
{
    os << LEFT_PAD 
       << std::setw(30) 
       << std::left 
       << _T(o.usuage + " ");

    os << o.description;
    return os;
}

} // namespace cli

#endif // CLI_LIB_OPTION_HPP
