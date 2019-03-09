/*
 * commands.h
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

bool cmd_help(int argc, char** argv);
bool cmd_encrypt(int argc, char** argv);
bool cmd_decrypt(int argc, char** argv);

void printEncHelp();

#endif /* COMMANDS_H_ */
