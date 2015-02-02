/*
 * VariablePool.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#include "VariablePool.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
using namespace std;

VariablePool::VariablePool() {
	this->counter = 0;
}

VariablePool::~VariablePool() {
}

char* VariablePool::generateVariable() {
	int postfix = this->counter;

	char *newVar = (char*) malloc(5);
	strcpy(newVar, "_var");

	int digits = 0;
	do { postfix /= 10; digits++; } while (postfix != 0);

	char *append = (char*) malloc(digits);
	sprintf(append, "%d", this->counter);
	newVar = (char*) realloc(newVar, strlen(newVar) + strlen(append));
	strcat(newVar, append);

	free(append);

	this->counter++;

	return newVar;
}
