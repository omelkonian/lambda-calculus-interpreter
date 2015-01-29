/*
 * error_handling.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ERROR_HANDLER_ERROR_HANDLING_H_
#define ERROR_HANDLER_ERROR_HANDLING_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

void print_error(char *command, const char *errorMessage, int position) {
	position += 2;
//	cout << command << endl;
	for (int i = 0; i < position; i++)
		cout << " ";
	cout << "^" << endl;
	for (int i = 0; i < position; i++)
		cout << " ";
	cout << errorMessage << endl;

	exit(-1);
}

#endif /* ERROR_HANDLER_ERROR_HANDLING_H_ */
