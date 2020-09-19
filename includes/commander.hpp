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
#include <regex>
#include <iomanip>
#include <map>
#include <Exception.hpp>
#include <Option.hpp>
#include <helper.hpp>
#include <fryday/debug.h>

namespace cli
{
    
class Commander
{
    /*
    |   This stores all the properties, which are accessable by externel user
    |   tho this is not directly visible to user, but can be read using over-
    |   loaded [] operator for this class.
    */
    std::map<std::string, std::string> properties;

    /*
    |   this store all the user defined options and their values for the program, 
    |   these values are updated when parse method is called.
    */
    std::map<Option, std::string> options;

    /*
    |   This stores the list of all commands for the program,
    |   TODO -> update this to a faster data structure for searching 
    */
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

void Commander::command(const std::string cmd, const std::string description)
{
    if (!cmd.length())
        throw Exception("command cannot be empty");

    // split the string from spaces 
    std::regex regex{R"([\s]+)"}; // split string with space 
    std::sregex_token_iterator it{cmd.begin(), cmd.end(), regex, -1};
    std::vector<std::string> words{it, {}};

    std::string key;

    // if (words.size() >= 1) key = words[0];
    // if (words.size() >= 2) action.m.insert({words[1], "inserted"});
    // if (words.size() >= 3) action.m.insert({words[2], "inserted"});

    Command command{key, cmd, description};
    this->commands.push_back(command);
}

void Commander::list() const noexcept
{
    std::cout << "\nList of available options and commands\n\n";

    // for (auto &command : this->commands)
    // {
    //     std::cout << "\t"
    //               << std::left
    //               << std::setw(15)
    //               << command.command
    //               << "\t"
    //               << command.description
    //               << '\n';
    // }
    std::cout << std::endl;
    for (auto &el : this->options)
    {
        std::cout << el.first << std::endl;
    }
    // std::cout << std::endl;
}

std::string Commander::operator[](const std::string key) const noexcept
{
    auto itr = this->properties.find(key);
    if (this->properties.find(key) != this->properties.end()) 
        return itr->second;
    
    return "";
}

} // namespace Commander

#endif // CLI_COMMANDER_HPP
