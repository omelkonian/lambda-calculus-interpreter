/*
 * error_handling.cpp
 *
 *  Created on: 30 …·Ì 2015
 *      Author: Orestis Melkonian
 */

#include "error_handling.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void print_error(const char *errorMessage, int position) {
	position += 2; // due to '> ' prompt.
	for (int i = 0; i < position; i++)
		cout << " ";
	cout << "^" << endl;
	for (int i = 0; i < position; i++)
		cout << " ";
	cout << errorMessage << endl;

	exit(-1);
}
