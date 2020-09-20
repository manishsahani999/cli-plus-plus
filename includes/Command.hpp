// -*- C++ -*-
//===--------------------------- command.hpp ------------------------------===//
//  
//  Copyright (c) 2020 Manish sahani
// 
//  This program is free software: Licensed under the MIT License. you may not 
//  use this file except in compliance with the License. You may obtain a copy 
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//===----------------------------------------------------------------------===//

#ifndef CLI_COMMAND_HPP
#define CLI_COMMAND_HPP

#include <string>
#include <vector>
#include <ostream>
#include <iomanip>
#include <helper.hpp>

namespace cli
{

class Command 
{
    /**
     * @brief 
     * 
     */
    int required = 0;

    /**
     * @brief 
     * 
     */
    std::string command;

    /**
     * @brief 
     * 
     */
    std::vector<std::string> argv; 

    /**
     * @brief 
     * 
     */
    std::string description;

public:
    Command(const std::string command, const std::string description = "");

    void handleArg(std::string str);
    int getRequired() const noexcept
    {
        return this->required;
    }

    bool operator<(const Command & command) const;
    friend std::ostream& operator<<(std::ostream & os, const Command & command);
};

 /**
 * @brief Construct a new Command object
 * 
 * @param command 
 * @param description 
 */
Command::Command(const std::string command, const std::string description) 
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

/**
 * @brief handles the Arg
 * 
 * @param str 
 */
void Command::handleArg(std::string str)
{
    if (str.front() == '<') required++;

    str.pop_back();
    str.erase(0, 1);
    this->argv.push_back(str);
}

/**
 * @brief Overloading the < operator for the Command Class
 * 
 * @param command 
 * @return true 
 * @return false 
 */
bool Command::operator<(const Command &command) const
{
    return command.command > this->command;
}

/**
 * @brief Overloading the << operator for the Command Class
 * 
 * @param os 
 * @param command 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream & os, const Command & command)
{
    os << "\t" << std::setw(10) << command.command << "\t";
    for(auto el : command.argv) os << "[" << el << "] ";
    os << "\t" << command.description << std::endl;
    return os;
}

}

#endif // CLI_COMMAND_HPP 
