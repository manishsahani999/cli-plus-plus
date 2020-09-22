# cli-plus-plus
Cli++ is a c++ framework for building commmand line app with expressive, elegant syntax. I’ve already laid the foundation — freeing you to create awesome cli applications without sweating the small things.

### Usage 
```c++
#include <iostream>
#include <cli++/commander.hpp>
#include <cli++/exception.hpp>

using namespace std;
using namespace cli;

int32_t main(int argc, char *argv[])
{
    Commander commander;

    try
    {
        commander.version("1.0");
        commander.option("-d", "directory");
        commander.option("-f", "filename");
        commander.command("version", "display the version of the cli tool");
        commander.command("clone <url> [path]", "clone the repository");
    }
    catch (const cli::Exception &e)
    {
        std::cerr << e.what() << '\n';
        abort();
    }
    
    if (commander.command == "version") {
        cout << commander["version"] << endl;
        return 0;
    }

    if (commander.command == "clone") {
        cout << "Cloning from "  << commander["url"] << endl;
        // do actual cloning
        return 0;
    }
    
    return 0;
}
```

![image](docs/terminal.png)