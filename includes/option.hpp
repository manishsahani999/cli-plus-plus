
#ifndef CLI_LIB_OPTION_HPP
#define CLI_LIB_OPTION_HPP

#include <string>
#include <map>
#include <helper.hpp>
#include <ostream>

namespace cli
{

class Option
{
    int required = 0;
    std::string flag;
    std::string readable;
    std::string description;
    std::string arg;

public:
    Option(std::string flag, std::string description);
    std::string getArg() const;
    void setArg(std::string str);
    
    bool operator<(const Option &option) const;
    friend std::ostream& operator<<(std::ostream & os, const Option & o);
};

Option::Option(std::string flag, std::string description)
{
    // process the flag type. if the flag has arguments, then update the required
    // according to <> or [] provided
    const std::vector<std::string> tokenized = helper::tokenize(flag, std::regex(R"([\s|]+)"));

    this->description = description;
    if (tokenized.size() >= 1)
        this->flag = tokenized[0];
    if (tokenized.size() >= 2)
    {
        if (tokenized[0].front() == '-')
            this->readable = tokenized[1];
        else
            this->setArg(tokenized[2]);
    }
    if (tokenized.size() >= 3)
        this->setArg(tokenized[2]);
}

std::string Option::getArg() const 
{
    return this->arg;
}

void Option::setArg(std::string str)
{
    if (str.front() == '<')
        required = 1;

    str.pop_back();
    str.erase(0, 1);
    this->arg = str;
}

bool Option::operator<(const Option &option) const
{
    return option.flag > this->flag;
}

std::ostream& operator<<(std::ostream & os, const Option & o)
{
    os  << "Flag             : " << o.flag << "\n"
        << "Readable Flag    : " << o.readable << "\n"
        << "Arg              : " << o.arg << "\n"
        << "Required         : " << o.required << "\n"
        << "Description      : " << o.description << "\n\n";
    return os;
}

struct Command
{
    std::string key;
    std::string command;
    std::string description;
};

} // namespace cli

#endif // CLI_LIB_OPTION_HPP