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
		this->insertSymbolAt(this->getFirstCharPos(), '(');
		this->insertSymbolAt(this->getLastCharPos() + 1, ')');
		cout << "#1: " << command << endl;
	}
	// #2. No rightmost parenthesis
	else if (this->getFirstChar() == '(' && this->getLastChar() != ')') {
		this->insertSymbolAt(this->getLastCharPos() + 1, ')');
		cout << "#2: " << command << endl;
	}
	// #3. No leftmost parenthesis
	else if (this->getLastChar() == ')' && this->getFirstChar() != '(') {
		this->insertSymbolAt(this->getFirstCharPos(), '(');
		cout << "#3: " << command << endl;
	}

	// #4. No spaces in numeric expressions.
	for (int i = 1; i < (int) strlen(command) - 1; i++) {
		if (isOperator(command[i])) {
			if (command[i - 1] != 32 && isDigit(command[i - 1])) {
				this->insertSymbolAt(i, 32);
				i++;
			}
			if (command[i + 1] != 32 && isDigit(command[i - 1]))
				this->insertSymbolAt(i + 1, 32);

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
					} else if (Scanner::isValidVarSymbol(command[i])) {
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
						this->insertSymbolAt(ranges[0]->min + (offset++), '(');
					for (int i = 1; i < (int) ranges.size(); i++)
						this->insertSymbolAt(ranges[i]->max + (offset++), ')');

				}
			}
		}
	}

	return this->command;
}

void AutoCorrector::insertSymbolAt(int position, char symbol) {
	assert(position < (int ) strlen(command));
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

bool AutoCorrector::parBalanced(string term) {
	int leftParNo = 0, rightParNo = 0;
	for (int i = 0; i < (int) term.size(); i++) {
		leftParNo += (term[i] == '(') ? 1 : 0;
		rightParNo += (term[i] == ')') ? 1 : 0;
	}
	if (leftParNo == 0) return false;
	return (leftParNo == rightParNo);
}

string AutoCorrector::removeUnnecessaryParentheses(string term) {
//	cerr << "Checking ";
//	cerr << "\33[0;1;31m" << term << "\33[0m" << endl;

	if (!AutoCorrector::parBalanced(term))
		return term;

	int leftIndex = term.find_first_of('(');
	int rightIndex = AutoCorrector::getClosingPar(term, leftIndex);
	assert(rightIndex != -1);
	if (leftIndex == rightIndex - 1)
		return term;
	while (leftIndex != (int) term.npos) {
		bool leftIn = (term[leftIndex + 1] == '(') ? true : false;
		bool rightIn = (rightIndex - 1 == AutoCorrector::getClosingPar(term, leftIndex + 1)) ? true : false;

		bool simpleTerm1 = true;
		for (int i = leftIndex + 1; i < rightIndex; i++) {
			if (!(Scanner::isValidVarSymbol(term[i]) || Scanner::isValidDigit(term[i]) || term[i] == '[' || term[i] == ']')) {
				simpleTerm1 = false;
				break;
			}
		}

		bool list = ((term[leftIndex + 1] == '[') && (term[rightIndex - 1] == ']'));
		if (list) {
			for (int i = leftIndex + 2; i < rightIndex - 1; i++)
				if (term[i] == '[' || term[i] == ']') list = false;
		}

		bool replaced = false;
		if (leftIn && rightIn) {
			cout << "#1________________" << term.substr(leftIndex, rightIndex - leftIndex + 1) << endl;
			term.erase(leftIndex + 1, 1);
			term.erase(rightIndex - 1, 1);
			replaced = true;
		} else if (simpleTerm1 || list) {
			cout << "#2__________________" << term.substr(leftIndex, rightIndex - leftIndex + 1) << endl;
			term.erase(leftIndex, 1);
			term.erase(rightIndex - 1, 1);
			replaced = true;
		}
		if (!replaced) {
			term[leftIndex] = '$';
			term[rightIndex] = '#';
		}

		leftIndex = term.find_first_of('(', leftIndex + 1);
		if (leftIndex != (int) term.npos)
			rightIndex = AutoCorrector::getClosingPar(term, leftIndex);
	}

	for (int i = 0; i < (int) term.size(); i++) {
		if (term[i] == '$') term[i] = '(';
		else if (term[i] == '#') term[i] = ')';
	}

	return term;
}

int AutoCorrector::getClosingPar(string term, int openingPar) {
	int parCount = 1;
	for (int i = openingPar + 1; i < (int) term.size(); i++) {
		if ((term[i] == ')') && (--parCount == 0))
			return i;
		else if (term[i] == '(')
			parCount++;
	}
	return -1;
}
