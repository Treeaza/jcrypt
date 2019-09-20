/*
 * encrypt.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#include "encrypt.h"

#include <iostream>
#include <cstring>
#include <ctype.h>
#include "util.h"

using namespace std;

struct encryptionOrder;

//Encryption token prototype
struct encFuncToken {
	virtual void excecute(char*, int, encryptionOrder*) = 0;
	virtual const char* toString() = 0;

	virtual ~encFuncToken() = 0;

	encFuncToken* next = NULL;
	encFuncToken* previous = NULL;
};

encFuncToken::~encFuncToken() {

}

//Encoding functions
void enc_bitmask(char*, int, char);
void enc_reverseBytes(char*, int);

struct encryptionOrder {
	char bitmasks[16] = { 0 };
	int maskCount = 0;
	bool saveOrder = false;

	int length = 0;
	encFuncToken* head = NULL;
	encFuncToken* tail = NULL;

	bool valid = true;

	void addToTail(encFuncToken* toAdd) {
		if (length == 0) {
			head = toAdd;
			tail = toAdd;
			length = 1;
		} else {
			tail->next = toAdd;
			length++;
			tail = toAdd;
		}
	}

	void addToHead(encFuncToken* toAdd) {
		if (length == 0) {
			head = toAdd;
			tail = toAdd;
			length = 1;
		} else {
			toAdd->next = head;
			length++;
			head = toAdd;
		}
	}

	void printStack() {
		cout << "The encryption stack now contains:\n";
		cout << "    Bitmasks:\n";
		for (char i = 0; i < 16; i++) {
			cout << "    -" << (char) (i <= 9 ? i + '0' : i + 'a' - 10) << ": "
					<< bitmasks[i - '0'] << "\n";
		}
		cout << "    Order:\n.....";
		cout << "checking if head is null";
		if (head != NULL) {
			cout << "head not null";
			encFuncToken* on = head;
			for (int i = 0; i < length; i++) {
				cout << "I AM A THING YES.";
				cout << "    -" << on->toString() << "\n";
			}
		}
	}
};

//Encryption tokens
struct maskFuncToken: encFuncToken {
	int bitmaskID;

	void excecute(char* txt, int length, encryptionOrder* order) {
		char mask = order->bitmasks[bitmaskID];
		enc_bitmask(txt, length, mask);
	}

	const char* toString() {
		return (char *) "Mask " + bitmaskID;
	}

	maskFuncToken(char ID) {
		bitmaskID = ID;
	}
};

struct reverseFuncToken: encFuncToken {
	void excecute(char* txt, int length, encryptionOrder* order) {
		enc_reverseBytes(txt, length);
	}

	const char* toString() {
		return "Reverse";
	}
};

struct standardMaskFuncToken: encFuncToken {
	void excecute(char* txt, int length, encryptionOrder* order) {
		enc_bitmask(txt, length, STANDARD_BITMASK);
	}

	const char* toString() {
		return "Mask std";
	}
};

//Prototypes for initialization functions
encryptionOrder readArguments(char**, int);
void readOrder(encryptionOrder*, char*);
void readBitmask(encryptionOrder*, char*);

bool encrypt(char* text, int length, int argc, char** argv) {
	cout << "EncFunc";
	//Arguments were supplied, let's read them and build a stack out of them.
	encryptionOrder stack = readArguments(argv, argc);

	stack.printStack();
	cout << "HELLO YES WE ARE HERE THAT IS GOOD";
	if (stack.valid) {
		//Now we need to go through and run the stack.
		encFuncToken* tk = stack.head;
		for (int i = 0; i < stack.length; i++) {
			tk->excecute(text, length, &stack);
			tk = tk->next;
		}
	} else {
		cout << "Encrytion arguments found to be invalid, aborting...";
		return false;
	}
	return true;
}

/**
 *	@returns an encryptionOrder describing how to properly encrypt according to these args
 */
encryptionOrder readArguments(char** argv, int argc) {
	cout << "EncFunc";

	//There can only be 16 bitmasks, fill them with 0s for now, to be safe.
	encryptionOrder order;
	//return order; //<---------

	for (int argNum = 0; argNum < argc; argNum++) {
		char* arg = argv[argNum];
		//-w args are ignored here, that's the command's problem.
		if (strcmp(arg, "-w") == 0) {
			//We do need to add one to argNum though to skip past the filename.
			argNum++;
		} else if (strcmp(arg, "-auto") == 0) {
			order.saveOrder = true;
		} else if (strcmp(arg, "-o") == 0) {
			//Oh yay.... go make another function do this.
			//(And increment the argNum to make sure we don't read the order as an argument.
			readOrder(&order, argv[++argNum]);
		} else if (arg[0] == '"') {
			//Found a bitmask, decode it and add it to the stack.
			readBitmask(&order, arg);
		} else {
			//Invalidate the stack, unrecognized args
			cout << "**Warning**\nUnrecognized arg " << arg
					<< " found, aborting.\n";
			order.valid = false;
		}
	}

	//If we got here without finding an order command, the order should be a single standard bitmask.
	if (order.length == 0) {
		//I could just plug it in directly but there's a function for that!
		char* stdCopy = new char[strlen(STANDARD_BITMASK_CODE)];
		stdCopy = strcpy(stdCopy, STANDARD_BITMASK_CODE);
		readBitmask(&order, stdCopy);
		delete[] stdCopy;
	}

	return order;
}

void readOrder(encryptionOrder* order, char* orderString) {
	bool valid = true;

	char c = tolower(orderString[0]);
	int i = 1;
	while (c != '\0') {
		//There's a character here, let's decode it.
		encFuncToken* tk;
		if (c == 'r') {
			//Reverse bytes, easy enough.
			reverseFuncToken t = reverseFuncToken();
			tk = &t;
		} else if (c == 's') {
			//Standard bitmask.... we should probably work out what that is.
			//Okay now it's a constant. And very secure, of course.
			standardMaskFuncToken t = standardMaskFuncToken();
			tk = &t;
		} else if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')) {
			//Convert the hex to numbers
			if (c < 'a')
				c -= '0';
			else
				c -= ('a' - 10);
			maskFuncToken t = maskFuncToken(c);
			tk = &t;
		} else {
			//For now, any other characters are invalid. Let's just invalidate the whole thing then.
			valid = false;
			break;
		}
		order->addToTail(tk);
		c = tolower(orderString[i++]);
	}

	if (!valid) {
		cout << "**Warning**\nInvalid order detected: " << orderString << endl;
		order->valid = false;
	}
}

void readBitmask(encryptionOrder* order, char* mask) {
	int length = 0;
	char maskDigits[8];
	memset(maskDigits, 0, 8);

	//Ignore the first character, should be ".
	//TODO: Tell the user if they screwed up a bitmask
	int on = 1;
	while (mask[on] != '"') {
		maskDigits[length++] = mask[on++];
	}

	bool valid = true;
	char maskByte = 0;

	switch (length) {
	case 2:
		//Hex - both chars should be 0-9 or a-f - let's make sure letters are uppercase to be safe
		for (int i = 0; i < 2; i++) {
			char dig = toupper(maskDigits[i]);
			if (dig < '0' || (dig > '9' && dig < 'A') || dig > 'F') {
				valid = false;
				break;
			}
			if (dig <= '9') {
				dig -= '0';
			} else {
				dig -= ('A' - 10);
			}
			maskByte += dig * (i == 1 ? 16 : 1);
		}
		break;
	case 3:
		//Dec - everything should be 0-9 here
		//Similar to above, we'll loop through and build up the mask byte.
		for (int i = 0; i < 3; i++) {
			char dig = maskDigits[i];
			if (dig < '0' || dig > '9') {
				valid = false;
				break;
			}
			dig -= '0';
			// We start with the leftmost digit, so this has to be inverted.
			maskByte += dig * (powi(10, 2 - i));
		}
		break;
	case 8:
		//Binary - similar again, digits can only be 1 or 2.
		for (int i = 0; i < 8; i++) {
			char dig = maskDigits[i];
			if (dig != '0' && dig != '1') {
				valid = false;
				break;
			}
			dig -= '0';
			//Inverted again
			maskByte += dig * (powi(2, 7 - i));
		}
		break;
	default:
		//Invalid, maybe warn them?
		valid = false;
		break;
	}

	if (!valid) {
		cout << "**Warning**\nInvalid mask detected: " << mask << endl;
		//If this is invalid, it's probably safer to return than write what we generated to the mask.
		//Actually just tell the whole stack it's invalid too
		order->valid = false;
		return;
	}

	order->bitmasks[order->maskCount++] = maskByte;

}

void enc_reverseBytes(char* txt, int length) {
	for (int i = 0; i < length; i++) {
		char ret = txt[i];
		for (int j = 0; j < 8; j++) {
			ret |= txt[i] & (1 << j);
		}
		txt[i] = ret;
	}
}

void enc_bitmask(char* txt, int length, char mask) {
	for (int i = 0; i < length; i++) {
		txt[i] ^= mask;
	}
}

