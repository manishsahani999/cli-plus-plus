#include <iostream>
#include <iomanip>
#include <regex>
#include "commander.hpp"
#include "exception.hpp"

using namespace Fryday;

void Fryday::Commander::version(const std::string version) noexcept
{
    this->properties["version"] = version;
}

void Fryday::Commander::option(const std::string flag, const std::string description)
{
    if (!flag.length())
        throw Fryday::Exception("option is missing flags");

    Fryday::Option option{flag, description};
    this->options.push_back(option);

    sort(this->options.begin(), this->options.end(), [](Option a, Option b) -> bool {
        return a.flag < b.flag;
    });
    // printf("Setting option %s \n", options[options.size() - 1].flag.c_str());
}

void Fryday::Commander::command(const std::string cmd, const std::string description, Fryday::Action &action)
{
    if (!cmd.length())
        throw Fryday::Exception("command cannot be empty");

    // split the string from spaces 
    std::regex regex{R"([\s]+)"}; // split string with space 
    std::sregex_token_iterator it{cmd.begin(), cmd.end(), regex, -1};
    std::vector<std::string> words{it, {}};

    std::string key;

    if (words.size() >= 1) key = words[0];
    if (words.size() >= 2) action.m.insert({words[1], "inserted"});
    if (words.size() >= 3) action.m.insert({words[2], "inserted"});

    Command command{key, cmd, description};
    this->commands.push_back(command);
}

void Fryday::Commander::list() const noexcept
{
    std::cout << "\nList of available options and commands\n\n";

    for (auto &command : this->commands)
    {
        std::cout << "\t"
                  << std::left
                  << std::setw(15)
                  << command.command
                  << "\t"
                  << command.description
                  << '\n';
    }
    std::cout << std::endl;

    for (auto &option : this->options)
    {
        std::cout << "\t"
                  << std::left
                  << std::setw(5)
                  << option.flag
                  << "\t"
                  << option.description
                  << '\n';
    }
    std::cout << std::endl;
}

std::string Fryday::Commander::operator[](const std::string key) const noexcept
{
    auto itr = this->properties.find(key);
    if (this->properties.find(key) != this->properties.end()) 
        return itr->second;
    
    return "";
}