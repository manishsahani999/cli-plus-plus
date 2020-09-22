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

#include <iostream>
#include <string>
#include <regex>
#include <iomanip>
#include <map>
#include <vector>
#include <command.hpp>
#include <exception.hpp>
#include <helper.hpp>
#include <option.hpp>
#include <colors.hpp>

namespace cli
{
    
class Commander
{
    // Name and description for the program 
    std::string name, description;

    /**
     * @brief This stores all the properties, which are accessable by externel user
     * tho this is not directly visible to user, but can be read using over- loaded 
     * [] operator for this class.
     */
    std::map<std::string, std::string> properties;

    /**
     * @brief this store all the user defined options and their values for the 
     * program, these values are updated when parse method is called, or when a ver
     * -sion option is registered.
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
    Commander(const std::string & n, const std::string & d = ""):name(n),description(d){
        this->option("-h|--help", "Display this help message");
    }

    // Commander configuration building api
    void version(const std::string & version, 
                 const std::string & flag = "-v| --version", 
                 const std::string & description = "display programs for version");
    void option(const std::string & flag, const std::string & description = "");
    void command(const std::string & command, const std::string & description = "");

    // Commander usage api
    void parse(int argc, char *argv[]);
    void list() const noexcept;

    std::string operator[](const std::string key) const noexcept;
};

/**
 * @brief Update the program's version info
 * 
 * @param version 
 * @param flag 
 * @param description 
 */
void Commander::version(const std::string & version, 
                        const std::string & flag, 
                        const std::string & description) 
{
    this->properties.insert({properties::VERSION, version});
    this->option(flag, description);
}

/**
 * @brief add a new option to the program 
 * 
 * @param flag 
 * @param description 
 * @throw cli::Exception 
 */
void Commander::option(const std::string & flag, 
                       const std::string & description)
{
    // check if the flag is empty or not, in any case flag must not be empty
    if (!flag.length()) throw Exception(errstr::option::FLAG_EMPTY);

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
void Commander::command(const std::string & cmd, const std::string & description)
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
    if (this->option_args.size() && this->properties.find(properties::VERSION) != this->properties.end()
            && (*this->option_args.begin() == "--version" || *this->option_args.begin() == "-v" ))
    {
        std::cout << this->properties[properties::VERSION] << std::endl;
        std::exit(0);
    }

    // This is to obey the legacy of -h and --help usage if --version of -v 
    if (this->option_args.size() && this->properties.find(properties::VERSION) != this->properties.end()
            && (*this->option_args.begin() == "--help" || *this->option_args.begin() == "-h" ))
    {
        this->list();
        std::exit(0);
    }

    // Identify and process commands 
    if (this->command_args.size() && 
        this->commands.find(*this->command_args.begin()) != this->commands.end())
    {
        std::string cmd_name = this->command_args.front();
        auto command = this->commands.find(cmd_name)->first;

        // Update properties and pop out the first element which was command name
        this->properties.insert({"command", cmd_name});
        this->command_args.erase(command_args.begin(), command_args.begin() + 1);

        //Validate Command args and Update the properties 
        if (command.validate(this->command_args.size())) 
        {
            std::vector<std::string> keys = command.getargv();
            for(int i = 0; i < this->command_args.size() && i < keys.size(); i++)
                this->properties.insert({keys[i], this->command_args[i]});
        }
        else {
            std::cout << command << std::endl;
            throw Exception(Static::ExceptionStr::PARSE_CMD_MISSING_ARG);
        }
    }
    else throw Exception(Static::ExceptionStr::PARSE_CMD_NOT_FOUND);

}

void Commander::list() const noexcept
{
    std::cout << "\n" << LEFT_PAD  << _P(this->name) << " " 
              << this->description << "\n";

    std::cout << "\nAvailable commads:\n";
    for (auto &command : this->commands) std::cout << command.first << std::endl;

    std::cout << "\nAvailable options:\n";
    for (auto &el : this->options) std::cout << el.first << std::endl;
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
