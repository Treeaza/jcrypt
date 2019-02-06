/*
 * encrypt.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#include "encrypt.h"

using namespace std;

void encrypt(char* text, int length, int argc, char** argv) {
	if (argc == 0) {
		//Run standard encrypt, return.
		cout << text << endl;
		for (int i = 0; i < length; i++) {
			text[i] = text[i] ^ 0b11101111;
		}
		//text is now all bitmasked, return so the command can write this
		return;
	}
}
