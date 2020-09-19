#include <iostream>
#include <commander.hpp>

using namespace std;
using namespace cli;

int32_t main(int argc, char *argv[])
{
    Commander program;

    try
    {
        program.version("1.0");
        program.option("-a", "simplest option");
        program.option("-b|--boom", "with aliases");
        program.option("-c|--cool <name>", "with required");
        program.option("-d|--doom [party]", "optional");
        program.option("-de| --doom [party]", "errored");
        // program.command("clone <url> [path]", "clone the repository");
        program.list();
    }
    catch (const cli::Exception &e)
    {
        std::cerr << e.what() << '\n';
        abort();
    }
  
    // cout << program["version"] << endl;
    
    return 0;
}