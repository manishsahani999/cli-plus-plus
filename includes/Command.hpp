// -*- C++ -*-
//===------------------------- command.hpp ------------------------------===//
//
//  Part of dotfiles management tool
//
//===----------------------------------------------------------------------===//

#ifndef CLI_COMMAND_HPP
#define CLI_COMMAND_HPP

#include "helper.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace cli
{

class Command 
{
    int required = 0;
    std::string command;
    std::vector<std::string> argv; 
    std::string description;

public:
    Command(std::string command, std::string description);

    void handleArg(std::string str);
    bool operator<(const Command & command) const;
    friend std::ostream& operator<<(std::ostream & os, const Command & c);
};

Command::Command(std::string command, std::string description) 
{
    // process the command. If the command has args, the resolve the required count 
    // depending on <> and []
    const std::vector<std::string> tokenized = helper::tokenize(command, std::regex(R"([\s]+)"));

    this->description = description;
    this->command = tokenized[0];
    for (int i = 1; i < tokenized.size(); i++) {
        this->handleArg(tokenized[i]);
    }
}

void Command::handleArg(std::string str)
{
    if (str.front() == '<') required++;

    str.pop_back();
    str.erase(0, 1);
    this->argv.push_back(str);
}

bool Command::operator<(const Command &command) const
{
    return command.command > this->command;
}

std::ostream& operator<<(std::ostream & os, const Command & c)
{
    os  << "Command          : " << c.command << "\n"
        << "Arg              : ";
    
    for(auto el : c.argv) os << el << ", ";

    os  << "\nDescription      : " << c.description << "\n\n";
    return os;
}

}

#endif // CLI_COMMAND_HPP 
