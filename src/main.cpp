
// -*- C++ -*-
//===------------------------------ main.cpp -------------------------------===//
//
//  Copyright (c) 2020 Manish sahani
//
//  This program is free software: Licensed under the MIT License. you may not
//  use this file except in compliance with the License. You may obtain a copy
//  of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <filesystem>
#include <commander.hpp>

#define LINUX 1

using namespace std;
using namespace cli;

/**
 *  This is an example demonstration to use the package, and is not included in
 *  the library.   
 */
int32_t main(int argc, char *argv[])
{
    Commander program("dotfiles", "is a tool to manage dot files with the.");

    try
    {
        program.version("1.0");
        program.help();

        program.command("activate <commit>", "add a file to an env.");
        program.command("add <path>", "add a file to an env.");
        program.command("commit", "list the commands available");
        program.command("init", "initiate the management of dotfiles.");

        program.option("-m <message>", "provide a message to the commit");
        program.option("-b, --boom", "with aliases");
        // program.option("-c, --cool <name>", "with required");
        // program.option("-d|--doom [party]", "optional");
        // program.option("-de| --doom [party]", "errored");
        // program.command("clone <url> [path]", "clone the repository");
        program.parse(argc, argv);
    }
    catch (const Exception &e)
    {
        std::cerr << e.what() << '\n';
        exit(1);
    }

    if (program["command"] == "init")
    {
        cout << _P("> ") << "Initiating the tracking and management of dotfiles\n";
    }

    if (program["command"] == "activate")
    {
        cout << _P("> ") << "Activate the commit " << _S(program["commit"]) << "\n";
    }

    return 0;
}