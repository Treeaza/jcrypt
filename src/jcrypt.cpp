//============================================================================
// Name        : jcrypt.cpp
// Author      : Jasper Rubin
// Version     : 1.0
// Copyright   : Copyright 2018, all rights reserved
// Description : Very very basic encryption of files, not meant to be hard to crack, just unreadable
//============================================================================

#include <iostream>
#include <map>
#include "commands.h"
#include "util.h"
using namespace std;

typedef bool (*argFunc)(int, char **);

map<string, argFunc> argMap = { { "help", &cmd_help }, { "enc", &cmd_encrypt },
		{ "dec", &cmd_decrypt } };

int main(int argc, char** argv) {
	if (argc == 1) {
		//No arguments were passed in, just tell the user about the program
		//TODO: Read this speech from a file
		cout
				<< "jcrypt v1.0 is installed and working.\nRun \"jrcypt help\" for help.";
		return 0;
	}

	//If we got here the user supplied arguments, look it up in the map and then run it
	//We hand the functions the argument list, minus "jrcypt <command>"

	argMap.at(argv[1])(argc - 2, &argv[2]);

	return 0;
}

