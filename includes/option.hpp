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
#include <helper.hpp>
#include <ostream>

namespace cli
{

class Option
{
    int required = 0;
    std::string flag;
    std::string readable;
    std::string description;
    std::string arg;

public:
    Option(std::string flag, std::string description);
    std::string getArg() const;
    void setArg(std::string str);
    
    bool operator<(const Option &option) const;
    friend std::ostream& operator<<(std::ostream & os, const Option & o);
};

Option::Option(std::string flag, std::string description)
{
    // process the flag type. if the flag has arguments, then update the required
    // according to <> or [] provided
    const std::vector<std::string> tokenized = helper::tokenize(flag, std::regex(R"([\s|]+)"));

    this->description = description;
    if (tokenized.size() >= 1)
        this->flag = tokenized[0];
    if (tokenized.size() >= 2)
    {
        if (tokenized[1].front() == '-')
            this->readable = tokenized[1];
        else
            this->setArg(tokenized[1]);
    }
    if (tokenized.size() >= 3)
        this->setArg(tokenized[2]);
}

std::string Option::getArg() const 
{
    return this->arg;
}

void Option::setArg(std::string str)
{
    if (str.front() == '<')
        required = 1;

    str.pop_back();
    str.erase(0, 1);
    this->arg = str;
}

bool Option::operator<(const Option &option) const
{
    return option.flag > this->flag;
}

std::ostream& operator<<(std::ostream & os, const Option & o)
{
    os << "\t" << std::setw(7) 
       << o.flag << " "
       << (o.readable.length() ? o.readable : "\t") << std::setw(10)
       << (o.arg.length() ? "[" + o.arg + "]" : "")   
       << "\t" << o.description;
    return os;
}

} // namespace cli

#endif // CLI_LIB_OPTION_HPP
