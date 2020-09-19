// -*- C++ -*-
//===------------------------- exception.hpp ------------------------------===//
//
//  Part of dotfiles management tool
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