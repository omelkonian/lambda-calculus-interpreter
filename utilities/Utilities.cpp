/*
 * Utilities.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#include "Utilities.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

Utilities::Utilities() {
}

Utilities::~Utilities() {
}

char* Utilities::insertAt(char* command, char* toInsert, int position) {
	assert(position < (int ) strlen(command));
	char *newCommand = (char*) malloc(strlen(command) + strlen(toInsert) + 1);

	int i = 0;
	while (i < position)
		newCommand[i] = command[i++];

	int j = 0;
	while (j < (int) strlen(toInsert))
		newCommand[i + j] = toInsert[j++];

	while (i < (int) strlen(command) + strlen(toInsert))
		newCommand[i + j] = command[i++];

	newCommand[i] = '\0';

	free(command);
	return newCommand;
}

char* Utilities::remove(char* command, Range* range) {
	assert(range->min >= 0 && range->max < (int )strlen(command));
	int removeLen = range->max - range->min + 1;
	char *newCommand = (char*) malloc(strlen(command) - removeLen + 1);

	int i = 0;
	while (i < range->min)
		newCommand[i] = command[i++];

	while (i < (int)strlen(command) - removeLen) {
		newCommand[i] = command[i + removeLen];
		i++;
	}

	newCommand[i] = '\0';
	free(command);
	return newCommand;
}
