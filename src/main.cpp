#include <iostream>
#include "commander.hpp"
#include "exception.hpp"

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
        commander.command("clone <url> [path]", "clone the repository");
    }
    catch (const cli::Exception &e)
    {
        std::cerr << e.what() << '\n';
        abort();
    }
    // commander.list();
    // cout << a["<url>"] << endl;
    // cout << argc << endl;
    // for (int i = 0; i < argc; i++)
    // {
    //     cout << argv[i] << endl;
    // }
    cout << commander["version"] << endl;
    
    return 0;
}