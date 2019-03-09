/*
 * encrypt.h
 *
 *  Created on: Feb 3, 2019
 *      Author: Jasper Rubin
 */

#ifndef ENCRYPT_H_
#define ENCRYPT_H_

//Very secure, of course. You can literally read it on Github.
const char STANDARD_BITMASK = 0b01110101;
const char STANDARD_BITMASK_CODE[] = "s";

bool encrypt(char*, int, int, char**);

#endif /* ENCRYPT_H_ */
