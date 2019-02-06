/*
 * util.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#include "util.h"

using namespace std;

bool checkConfirmation(const char* msg, bool easy) {
	cout << msg << " (YES/NO)\n";
	char* n = new char[3];
	cin >> n;

	if (easy) {
		toUpperStr(n);
		return (strcmp(n, "YES") == 0 || strcmp(n, "Y") == 0);
	}

	return strcmp(n, "YES") == 0;
}

void toUpperStr(char* str) {
	for (int i = 0; str[i] == '\0'; i++) {
		str[i] = toupper(str[i]);
	}
}
