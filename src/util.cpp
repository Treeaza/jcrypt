/*
 * util.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#include "util.h"

#include <iostream>
#include <string.h>

using namespace std;

bool checkConfirmation(const char* msg, bool easy) {
	cout << msg << " (YES/NO)\n\n";
	char* n = new char[3];
	cin >> n;
	cout << endl;

	if (easy) {
		toUpperStr(n);
		return (strcmp(n, "YES") == 0 || strcmp(n, "Y") == 0);
	}
	bool accepted = strcmp(n, "YES") == 0;

	delete [] n;

	return accepted;
}

void toUpperStr(char* str) {
	for (int i = 0; str[i] == '\0'; i++) {
		str[i] = toupper(str[i]);
	}
}

int powi(int base, int exponent) {
	int ret = 1;
	for (int i = 0; i < exponent; i++) {
		ret *= base;
	}
	return ret;
}
