// -*- C++ -*-
//===------------------------- commander.hpp ------------------------------===//
//  
//  Copyright (c) 2020 Manish sahani
// 
//  This program is free software: Licensed under the MIT License. you may not 
//  use this file except in compliance with the License. You may obtain a copy 
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//===----------------------------------------------------------------------===//

#ifndef CLI_COMMANDER_HPP
#define CLI_COMMANDER_HPP

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <map>
#include <exception.hpp>
#include <option.hpp>
#include <command.hpp>
#include <helper.hpp>
#include <fryday/debug.h>

namespace cli
{
    
class Commander
{
    /**
     * @brief This stores all the properties, which are accessable by externel user
     * tho this is not directly visible to user, but can be read using over- loaded 
     * [] operator for this class.
     */
    std::map<std::string, std::string> properties;

    /**
     * @brief this store all the user defined options and their values for the 
     * program, these values are updated when parse method is called.
     */
    std::map<Option, std::string> options;

    /**
     * @brief this stores the list of all the user's commands for the program, these
     * values are updated when the parse method is called. 
     */
    std::map<Command, std::string> commands;

    /**
     * @brief this stores the args at the provided at run time, updated by the parse 
     * method.
     */
    std::vector<std::string> option_args;

    /**
     * @brief this is to store commands args provided at the runtime, updated by the 
     * parser 
     */
    std::vector<std::string> command_args;

    // Helper functions 
    void populate(int arc, char *argv[]);
public:
    Commander(){};

    // Commander configuration building api
    void version(const std::string version) noexcept;
    void option(const std::string flag, const std::string description);
    void command(const std::string command, const std::string description);

    // Commander usage api
    void parse(int argc, char *argv[]);
    void list() const noexcept;

    std::string operator[](const std::string key) const noexcept;
};

/**
 * @brief Update the program's version info
 * 
 * @param version 
 */
void Commander::version(const std::string version) noexcept
{
    this->properties.insert({Static::VERSION, version});
}

/**
 * @brief add a new option to the program 
 * 
 * @param flag 
 * @param description 
 * @throw cli::Exception 
 */
void Commander::option(const std::string flag, const std::string description = "")
{
    // check if the flag is empty or not, in any case flag must not be empty
    if (!flag.length())
        throw Exception(Static::ExceptionStr::OPTION_FLAG_EMPTY);

    // Create an Option and insert in the global options  
    this->options.insert({Option(flag, description), ""});
}

/**
 * @brief add a new command to the program 
 *
 * @param cmd 
 * @param description
 * @throw cli::Exception 
 */
void Commander::command(const std::string cmd, const std::string description)
{
    // check if command string is empty or not, in any case cmd must not be empty
    if (!cmd.length())
        throw Exception("command cannot be empty");

    // Create an coommand and insert in the global commands 
    this->commands.insert({Command(cmd, description), ""});
}

/**
 * @brief Parse the input args in the programs
 * 
 * @param argc 
 * @param argv 
 */
void Commander::parse(int argc, char *argv[])
{
    // Update the this->args from provided arguemtns 
    this->populate(argc, argv);
    
    // If no arguments are provided, display the usage
    if (!this->option_args.size() && !this->command_args.size()) this->list();

    // This is to obey the legacy of -v and --version usage if --version of -v is 
    // provided and and this->properties has a version field then display the 
    // version
    if (this->option_args.size() && this->properties.find(Static::VERSION) != this->properties.end()
            && (*this->option_args.begin() == "--version" || *this->option_args.begin() == "-v" ))
    {
        std::cout << this->properties[Static::VERSION] << std::endl;
        std::exit(0);
    }

    // Identify and process commands 
    for(auto i : this->command_args) std::cout << i << "\n";
    std::cout << "--\n";
    for(auto i : this->option_args) std::cout << i << "\n";


    // if (this->args.size() >= 1) 
    // {
    //     this->properties.insert({"command", args[0]});
    //     auto it = this->commands.find(args[0]);
    //     if (it == this->commands.end()) 
    //     {
    //         std::cerr << Static::ExceptionStr::PARSE_CMD_NOT_FOUND << std::endl;
    //         exit(1);
    //     }
    //     // args[0];
    //     Command command = it->first;
    //     int argprovided = 0;
    //     for(int i = 1; i < args.size(); i++) {
    //         if (args[i].front() == '-') break;
    //         argprovided++;
    //     }
    //     std::cout << argprovided << " " << command.getRequired() << "\n";
    //     if (argprovided < command.getRequired()) 
    //     {
    //         std::cerr << "Invalid args" << std::endl;
    //         std::cout << command << std::endl;
    //         exit(1);
    //     }
    //     // for(int i = 1; i < args.size(); i++) {
    //     //     if (args[i].front() == '-') break;
    //     //     // this->properties.insert({command.})
    //     // }
    //     std::cout << command << std::endl;
    // }
}

void Commander::list() const noexcept
{
    std::cout << "\nList of available options and commands\n\n";

    for (auto &command : this->commands) std::cout << command.first << std::endl;
    std::cout << std::endl;
    
    for (auto &el : this->options) std::cout << el.first << std::endl;
    std::cout << std::endl;
    std::exit(0);
}

/**
 * @brief Overload [] for Commander
 * 
 * @param key 
 * @return std::string 
 */
std::string Commander::operator[](const std::string key) const noexcept
{
    auto itr = this->properties.find(key);
    if (this->properties.find(key) != this->properties.end()) 
        return itr->second;
    
    return "";
}

/**
 * @brief Populate the commander args from runtime provided args
 * 
 * @param argc 
 * @param argv 
 */
void Commander::populate(int argc, char *argv[]) 
{
    int i = 1;
    for(; i < argc && strlen(argv[i]) && argv[i][0] != '-'; i++) 
        this->command_args.push_back(std::string(argv[i]));

    for(; i < argc; i++) this->option_args.push_back(std::string(argv[i]));
}

} // namespace Commander

#endif // CLI_COMMANDER_HPP
