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

#include <iostream>
#include <commander.hpp>

using namespace std;
using namespace cli;

/**
 *  This is an example demonstration to use the package, and is not included in
 *  the library.   
 */
int32_t main(int argc, char *argv[])
{
    Commander program("dotfiles", "tool to manage dot files with the.");

    try
    {
        program.version("1.0");
        program.option("-a", "simplest option");
        program.option("-b, --boom", "with aliases");
        program.option("-c, --cool <name>", "with required");
        // program.option("-d|--doom [party]", "optional");
        // program.option("-de| --doom [party]", "errored");
        program.command("clone <url> [path]", "clone the repository");
        program.command("add", "list the commands available");
        program.command("commit", "list the commands available");
        program.parse(argc, argv);
    }
    catch (const cli::Exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }
    

    cout << program[properties::VERSION] << endl;
    
    return 0;
}