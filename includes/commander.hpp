// -*- C++ -*-
//===------------------------- commander.hpp ------------------------------===//
//
//  Part of dotfiles management tool
//
//===----------------------------------------------------------------------===//

#ifndef CLI_COMMANDER_HPP
#define CLI_COMMANDER_HPP

#include <string>
#include <vector>
#include <map>
#include "option.hpp"

namespace cli
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
        void command(const std::string command, const std::string description);
        void list() const noexcept;
        void parse(int argc, char *argv[]);

        std::string operator[](const std::string key) const noexcept;
    };
} // namespace Commander

#endif // CLI_COMMANDER_HPP