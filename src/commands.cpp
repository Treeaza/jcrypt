/*
 * commands.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#include "commands.h"

using namespace std;

bool cmd_decrypt(int argc, char** argv) {
	return true;
}

bool cmd_help(int argc, char** argv) {
	cout << "HELP HELP HELP!!!";
	return true;
}

bool cmd_encrypt(int argc, char** argv) {
	if (argc == 0) {
		//Interpret this as wanting help.
		printEncHelp();
		return true;
	}
	//They actually want to encrypt something? Shit, I guess we should get to it then. Start
	//by making sure that the file they want exists, and is valid, and load it.
	fstream file(argv[0], fstream::in | fstream::out);

	//If the file actually loaded we can keep going.
	if (file) {
		int length;
		char* txt;

		file.seekg(0, ios::end);
		length = file.tellg();
		txt = new char[length + 1];
		file.seekg(0, ios::beg);
		file.read(txt, length);

		cout << "File loaded, encrypting...\n\n";

		//txt should now contain the entire file in byte form. Now we can go encrypt it.
		encrypt(txt, length, argc - 1, &argv[1]);

		cout << "ENCRYPTION COMPLETE\n";

		bool confirmed = checkConfirmation("Do you want to overwrite the original file with this encrypted version now?");

		file.close();
		return true;
	}

	cout << "Invalid file name supplied. " << argv[0]
			<< " is not a .txt file or is not in this directory.\n";
	return false;
}

void printEncHelp() {
	cout
			<< "The enc (encrypt) command is used to encrypt your files.\n"
					"It is (so far) only compatible with .txt files, and must be used as follows:\n\n"
					"jcrypt enc \"<filename>\" <options>\n\n"
					"Options:\nNone: follows standard encryption scheme, masking by a predetermined key. Mainly just to obscure text.\n"
					"-w <filename>: instead of replacing the input file, output will be written to filename.\n"
					"-auto: encode the encryption path into the file, can be read by decryptor. This is terrible for security, but very convenient.\n"
					"-o <order>: encryption will be completed in the steps laid out in order. This same order can be handed to the decryptor to run backwards.\n"
					"     The order path can be given the following arguments:\n"
					"     0-9, a-f: mask each byte by a bitmask that is specified later. To use the first bitmask write a 1, and so on."
					"Bitmasks must be specified using the syntax provided later.\n"
					"     b: mask each byte by the standard bitmask, as used when no arguments are provided.\n"
					"     r: reverse the order of the bits in each byte.\n"
					"     To specify bitmasks, write the mask bytes in quotes, either in hex, binary, or decimal.\n"
					"     ie. to flip the full byte use \"ff\", \"11111111\", or \"255\". Anything else is invalid.\n"
					"     Bitmasks will be numbered in the order they appear in the command, and can be used accordingly."
					"\n\nHave fun!\n";
}

