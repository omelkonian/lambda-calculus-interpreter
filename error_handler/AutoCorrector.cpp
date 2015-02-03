/*
 * AutoCorrector.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Orestis Melkonian
 */

#include "AutoCorrector.h"
#include "../scanner/Scanner.h"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>

#include <iostream>

using namespace std;

class indexRange {
public:
	int min;
	int max;

	indexRange(int min, int max) {
		this->min = min;
		this->max = max;
	}
};

AutoCorrector::AutoCorrector(char *command) {
	this->command = command;
}

AutoCorrector::~AutoCorrector() {
}

bool isOperator(char symbol) {
	return ((symbol == 37) || (symbol == 42) || (symbol == 43) || (symbol == 45) || (symbol == 47)) ? true : false;
}

bool isDigit(char symbol) {
	return ((symbol >= 48) && (symbol <= 57)) ? true : false;
}

char* AutoCorrector::autoCorrect() {
	// #1. No out-most parentheses
	if ((this->getLastChar() != ')' && this->getFirstChar() != '(')) {
		this->insertAt(this->getFirstCharPos(), '(');
		this->insertAt(this->getLastCharPos() + 1, ')');
		cout << "#1: " << command << endl;
	}
	// #2. No rightmost parenthesis
	else if (this->getFirstChar() == '(' && this->getLastChar() != ')') {
		this->insertAt(this->getLastCharPos() + 1, ')');
		cout << "#2: " << command << endl;
	}
	// #3. No leftmost parenthesis
	else if (this->getLastChar() == ')' && this->getFirstChar() != '(') {
		this->insertAt(this->getFirstCharPos(), '(');
		cout << "#3: " << command << endl;
	}

	// #4. No spaces in numeric expressions.
	for (int i = 1; i < (int) strlen(command) - 1; i++) {
		if (isOperator(command[i])) {
			if (command[i - 1] != 32 && isDigit(command[i - 1])) {
				this->insertAt(i, 32);
				i++;
			}
			if (command[i + 1] != 32 && isDigit(command[i - 1]))
				this->insertAt(i + 1, 32);

		}
	}

	// #5. Check for an abstraction with multiple variables in the body without nested parentheses.
	for (int i = 1; i < (int) strlen(command) - 4; i++) {
		if (command[i] == '\\') {
			int max = (int) strlen(command);
			while (i + 1 < max && Scanner::isValidVarSymbol(command[++i])) {
			}



			if (i < max && command[i] == '.') {

				std::vector<indexRange*> ranges;
				indexRange *cur = NULL;
				char prev = command[i];

				while (i + 1 < max && command[++i] != ')') {
					if (command[i] == 32) {
						if (Scanner::isValidVarSymbol(prev)) {
							cur = NULL;
						}
						prev = 32;
					}
					else if (Scanner::isValidVarSymbol(command[i])) {
						if (prev == 32 || prev == '.') {
							cur = new indexRange(i, i + 1);
							ranges.push_back(cur);
						} else if (Scanner::isValidVarSymbol(prev))
							cur->max++;
						prev = command[i];
					}
				}

				if (ranges.size() > 1) {
					int offset = 0;
					for (int i = 1; i < (int) ranges.size(); i++)
						this->insertAt(ranges[0]->min + (offset++), '(');
					for (int i = 1; i < (int) ranges.size(); i++)
						this->insertAt(ranges[i]->max + (offset++), ')');

				}
			}
		}
	}

	return this->command;
}

void AutoCorrector::insertAt(int position, char symbol) {
	assert(position < (int) strlen(command));
	char *newCommand = (char*) malloc(strlen(command) + 2);

	int i = 0;
	while (i < position)
		newCommand[i] = command[i++];
	newCommand[i] = symbol;
	while (i <= (int) strlen(command))
		newCommand[i + 1] = command[i++];
	newCommand[i] = '\0';

	free(this->command);
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

int AutoCorrector::getLeftParNo() {
	int ret = 0;
	for (int i = 0; i < (int) strlen(this->command); i++)
		if (this->command[i] == '(')
			ret++;
	return ret;
}

int AutoCorrector::getRightParNo() {
	int ret = 0;
	for (int i = 0; i < (int) strlen(this->command); i++)
		if (this->command[i] == ')')
			ret++;
	return ret;
}
