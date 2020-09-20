// -*- C++ -*-
//===--------------------------- exception.hpp ----------------------------===//
//  
//  Copyright (c) 2020 Manish sahani
// 
//  This program is free software: Licensed under the MIT License. you may not 
//  use this file except in compliance with the License. You may obtain a copy 
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//===----------------------------------------------------------------------===//
#ifndef CLI_LIB_EXCEPTION_HPP
#define CLI_LIB_EXCEPTION_HPP

#include <string>

namespace cli
{
namespace Static
{
    static std::string VERSION = "version";

    namespace ExceptionStr
    {
        static std::string OPTION_FLAG_EMPTY = "Option flag cannot be empty, Provided an empty string.";
        static std::string PARSE_MISSING_CMD = "Command not provided";
        static std::string PARSE_CMD_NOT_FOUND = "Command not found";
    }
}

struct Exception
{
    const std::string str;
public:
    Exception(std::string s) : str(s) {}
    std::string what() const
    {
        return str;
    }
};

} // namespace Fryday

#endif