
#ifndef FRYDAY_CMD_LIB_OPTION_HPP
#define FRYDAY_CMD_LIB_OPTION_HPP

#include <string>
#include <map>

namespace Fryday
{

    struct Option
    {
        std::string flag;
        std::string description;
    };

    struct Action
    {
        std::map<std::string, std::string> m;
        std::string operator[](const std::string key) {
            if (m.find(key) != m.end()) return m[key];
            return "";
        }
    };
    
    struct Command
    {
        std::string key;
        std::string command;
        std::string description;
    };

} // namespace Fryday

#endif // FRYDAY_CMD_LIB_OPTION_HPP