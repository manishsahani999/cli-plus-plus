
#ifndef CLI_LIB_OPTION_HPP
#define CLI_LIB_OPTION_HPP

#include <string>
#include <map>

namespace cli
{

    struct Option
    {
        std::string flag;
        std::string description;
    };
    
    struct Command
    {
        std::string key;
        std::string command;
        std::string description;
    };

} // namespace Fryday

#endif // CLI_LIB_OPTION_HPP