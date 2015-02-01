/*
 * AutoCorrector.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Orestis Melkonian
 */

#include "AutoCorrector.h"
#include <stdlib.h>
#include <string.h>

#include <iostream>

using namespace std;

AutoCorrector::AutoCorrector(char *command) {
	this->command = command;
}

AutoCorrector::~AutoCorrector() {
}

bool isOperator(char symbol) {
	return ((symbol == 37) || (symbol == 42) || (symbol == 43) || (symbol == 45) || (symbol == 47)) ? true : false;
}

char* AutoCorrector::autoCorrect() {

	// #1. No outmost parentheses
	if (this->getLastChar() != ')' && this->getFirstChar() != '(') {
		this->insertAt(this->getFirstCharPos(), '(');
		this->insertAt(this->getLastCharPos() + 1, ')');

		cout << "#1: " << this->command << endl;
	}
	// #2. No rightmost parenthesis
	else if (this->getFirstChar() == '(' && this->getLastChar() != ')') {
		this->insertAt(this->getLastCharPos() + 1, ')');
		cout << "#2: " << this->command << endl;
	}
	// #3. No leftmost parenthesis
	else if (this->getLastChar() == ')' && this->getFirstChar() != '(') {
		this->insertAt(this->getFirstCharPos(), '(');
		cout << "#3: " << this->command << endl;
	}

	// #4. No spaces in numeric expressions.
	for (int i = 1; i < (int)strlen(this->command) - 1; i++) {
		if (isOperator(this->command[i])) {
			if (this->command[i - 1] != 32) {
				this->insertAt(i, 32);
				i++;
			}
			if (this->command[i + 1] != 32)
				this->insertAt(i + 1, 32);

		}
	}

	return this->command;
}

void AutoCorrector::insertAt(int position, char symbol) {
	char *newCommand = (char*) malloc(strlen(command) + 1);
	int i = 0;
	while (i < position)
		newCommand[i] = command[i++];
	newCommand[i] = symbol;
	while (i <= (int) strlen(command))
		newCommand[i + 1] = command[i++];
	delete this->command;
	this->command = newCommand;
}

char AutoCorrector::getFirstChar() {
	int i = 0;
	while (this->command[i] == 32)
		i++;
	return this->command[i];
}

int AutoCorrector::getFirstCharPos() {
	int i = 0;
	while (this->command[i] == 32)
		i++;
	return i;
}

char AutoCorrector::getLastChar() {
	int i = strlen(this->command) - 1;
	while (this->command[i] == 32)
		i--;
	return this->command[i];
}

int AutoCorrector::getLastCharPos() {
	int i = strlen(this->command) - 1;
	while (this->command[i] == 32)
		i--;
	return i;
}
