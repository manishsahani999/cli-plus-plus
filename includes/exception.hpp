// -*- C++ -*-
//===------------------------- exception.hpp ------------------------------===//
//
//  Part of dotfiles management tool
//
//===----------------------------------------------------------------------===//

#ifndef FRYDAY_CMD_LIB_EXCEPTION_HPP
#define FRYDAY_CMD_LIB_EXCEPTION_HPP

#include <string>

namespace Fryday
{
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