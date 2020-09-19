// -*- C++ -*-
//===------------------------- commander.hpp ------------------------------===//
//
//  Part of dotfiles management tool
//
//===----------------------------------------------------------------------===//

#ifndef FRYDAY_CMD_LIB_COMMANDER_HPP
#define FRYDAY_CMD_LIB_COMMANDER_HPP

#include <string>
#include <vector>
#include <map>
#include "option.hpp"

namespace Fryday
{
    class Commander
    {
        std::map<std::string, std::string> properties;
        std::vector<Option> options;
        std::vector<Command> commands;
    public:
        Commander(){};

        void version(const std::string version) noexcept;
        void option(const std::string flag, const std::string description);
        void command(const std::string command, const std::string description, Action & action);
        void list() const noexcept;
        void parse(int argc, char *argv[]);

        std::string operator[](const std::string key) const noexcept;
    };
} // namespace Commander

#endif // FRYDAY_CMD_LIB_COMMANDER_HPP