// -*- C++ -*-
//===------------------------- commander.hpp -----------------------------===//
//  
//  Copyright (c) 2020 Manish sahani
// 
//  This program is free software: Licensed under the MIT License. you may not 
//  use this file except in compliance with the License. You may obtain a copy 
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//===---------------------------------------------------------------------===//

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
     * @brief This stores all the properties, which are accessable by externel 
     * user tho this is not directly visible to user, but can be read using 
     * over- loaded [] operator for this class.
     */
    std::map<std::string, std::string> properties;

    /**
     * @brief this store all the user defined options and their values for the 
     * program, these values are updated when parse method is called, or when 
     * a version option is registered.
     */
    std::vector<Option> options;

    /**
     * @brief this stores the list of all the user's commands for the program,
     * these values are updated when the parse method is called. 
     */
    std::map<Command, std::string> commands;

    /**
     * @brief this stores the args at the provided at run time, updated by the
     * parse method.
     */
    std::vector<std::string> option_args;

    /**
     * @brief this is to store commands args provided at the runtime, updated 
     * by the parser 
     */
    std::vector<std::string> command_args;

    // Helper functions 
    void populate(int arc, char *argv[]);
    void parse_cmd();
    void parse_options();
    void is_cmd_version();
    void is_cmd_help();

public:
    Commander(const std::string & n, 
              const std::string & d = "") : name(n),description(d) {}

    //===-----------------------------------------------------------------===//
    //                                                                       //
    //  Commander's api for building the options and commands structure      //
    //                                                                       //
    //===-----------------------------------------------------------------===//

    /**
     * @brief Update the program's version info and register a version option
     * 
     * @param version 
     * @param flag 
     * @param description 
     */
    void version(const std::string & version, 
                 const std::string & flag = df::version_flag, 
                 const std::string & description = df::version_description);

    /**
     * @brief Register a help option to the program
     * 
     * @param flag 
     * @param description 
     */
    void help(const std::string & flag = df::help_flag, 
              const std::string & description = df::help_description);

    /**
     * @brief Register a new option to the program 
     * 
     * @param flag 
     * @param description 
     */
    void option(const std::string & flag, 
                const std::string & description = "");

    /**
     * @brief Register a new command to the program 
     * 
     * @param command 
     * @param description 
     */
    void command(const std::string & command, 
                 const std::string & description = "");

    //===-----------------------------------------------------------------===//
    //                                                                       //
    //  Commander's api for building the options and commands structure      //
    //                                                                       //
    //===-----------------------------------------------------------------===//

    /**
     * @brief Parse the input command line args to the commander,
     * 
     * @param argc 
     * @param argv 
     */
    void parse(int argc, char *argv[]);

    /**
     * @brief list the available commands and options of the program
     * 
     */
    void usage() const noexcept;

    std::string operator[](const std::string key) const noexcept;
};

/**
 * @brief Update the program's version info, and add version option 
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
 * @brief add -h help option in the program
 * 
 * @param flag 
 * @param description 
 */
void Commander::help(const std::string & flag, const std::string & description)
{
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
    this->options.push_back(Option(flag, description));
}

/**
 * @brief add a new command to the program 
 *
 * @param cmd 
 * @param description
 * @throw cli::Exception 
 */
void Commander::command(const std::string & cmd, 
                        const std::string & description)
{
    // check if command string is empty or not, cmd must not be empty
    if (!cmd.length()) throw Exception("command cannot be empty");

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
    if (!this->option_args.size() && !this->command_args.size()) this->usage();

    // This is to obey the legacy of -v|--version and -h|--help. 
    // usage if --version|-v is provided and and this->properties has a version 
    // field then display the version
    this->is_cmd_version();
    this->is_cmd_help();

    // Identify and process commands 
    this->parse_cmd();

    std::cout << "\n";
    for(auto el : this->option_args) std::cout << el << " \n ";
    std::cout << "\n";

    // Identify and process options 
    this->parse_options();
}

void Commander::usage() const noexcept
{
    std::cout << "\n" << LEFT_PAD  << _P(this->name) << " " 
              << this->description << "\n";

    std::cout << "\nAvailable commads:\n";
    for (auto &command : this->commands) std::cout << command.first 
                                                   << std::endl;

    std::cout << "\nAvailable options:\n";
    for (auto &el : this->options) std::cout << el << std::endl;
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

void Commander::is_cmd_version()
{
    if (this->option_args.size() && 
        this->properties.find(properties::VERSION) != this->properties.end() && 
        (*this->option_args.begin() == "--version" || 
         *this->option_args.begin() == "-v" ))
    {
        std::cout << this->properties[properties::VERSION] << std::endl;
        std::exit(0);
    }
}

void Commander::is_cmd_help()
{
    if (this->option_args.size() && this->properties.find(properties::VERSION) != this->properties.end()
            && (*this->option_args.begin() == "--help" || *this->option_args.begin() == "-h" ))
    {
        this->usage();
        std::exit(0);
    }
}

void Commander::parse_cmd()
{
    if (this->command_args.size())
    {
        std::string cmd_name = this->command_args.front();
        auto f = this->commands.find(cmd_name);

        if (f != this->commands.end())
        {
            auto command = f->first;
            // Update properties and pop out the first element which was command name
            this->properties.insert({properties::command, cmd_name});
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
    
}

void Commander::parse_options()
{
    if (this->option_args.size()) 
    {
        int i = 0;
        for(; i < this->option_args.size(); i++)
        {
            if (this->option_args[i].front() != '-') continue;
            std::cout << this->option_args[i] << "\n";
            Option *option = nullptr;
            // find the flag in the options 
            for (auto & el : this->options) if (el == this->option_args[i]) {
                option = &el;
                std::cout << " Hello \n ";
                break;
            }
            // if (option != nullptr) std::cout << option->getArg() << "\n";
            // if (it != this->option_args.end() && it->front() != '-') arg = *it;
            // --it;

            // auto f = this->options.find(flag);
            // if (f != this->options.end()) std::cout << f->first << std::endl;
        }
    }
    exit(0);
    
}

} // namespace Commander

#endif // CLI_COMMANDER_HPP
