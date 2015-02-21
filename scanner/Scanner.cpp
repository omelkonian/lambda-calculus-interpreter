/*
 * Scanner.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include "Scanner.h"
#include "../defines.h"
#include "../error_handler/error_handling.h"

#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>

using namespace std;

// Utility functions.
bool Scanner::isValidVarSymbol(char symbol) {
	// Valid Symbols: a-z, A-Z, _
	return (((symbol >= 65) && (symbol <= 90)) || ((symbol >= 97) && (symbol <= 122)) || (symbol == 95)) ? true : false;
}

bool Scanner::isValidDigit(char symbol) {
	// Valid digits: 0-9
	return ((symbol >= 48) && (symbol <= 57)) ? true : false;
}

bool Scanner::isValidOperator(char symbol) {
	// Valid operators: +, -, *, ^
	return ((symbol == 43) || (symbol == 45) || (symbol == 42) || (symbol == 94)) ? true : false;
}

Scanner::Scanner(char *command) {
	this->command = command;
	this->readPosition = 0;
	this->firstSymbolRead = false;
}

Scanner::~Scanner() {
}

Token* Scanner::nextToken() {
	char variableName[MAX_VARIABLE_LENGTH];
	int variableLen = 0;

	char number[MAX_NUMBER_DIGITS];
	int numberLen = 0;

	int maxReadPos = strlen(this->command);

	while (this->readPosition < maxReadPos) {

		char cur = this->command[this->readPosition];

		if ((!firstSymbolRead) && (cur != 32)) {
			firstSymbolRead = true;
			if (cur != '(') {
				print_error("Left parenthesis missing", this->readPosition - 1);
				return new Token(ERROR, 0, NULL);
			}
		}

		switch (cur) {
		// Whitespace
		case 32:
			this->readPosition++;
			continue;
			// Left parenthesis
		case '(':
			this->readPosition++;
			return new Token(LEFT_PAR, this->readPosition - 1, new TokenValue(NONE, NULL));
			// Right parenthesis
		case ')':
			this->readPosition++;
			return new Token(RIGHT_PAR, this->readPosition - 1, new TokenValue(NONE, NULL));
			// Backslash
		case 92:
			this->readPosition++;
			return new Token(LAMBDA, this->readPosition - 1, new TokenValue(NONE, NULL));
			// Dot
		case '.':
			this->readPosition++;
			return new Token(LAMBDA_DOT, this->readPosition - 1, new TokenValue(NONE, NULL));
		default:
			// Operator
			if (isValidOperator(cur) && (this->readPosition + 1 < maxReadPos) && !isValidDigit(this->command[this->readPosition + 1])) {
				this->readPosition++;
				return new Token(OPERATOR, this->readPosition - 1, new TokenValue(CHAR, &cur));
			}
			// Non-spaced operation
			else if ((isValidOperator(cur) && (cur != '-') && (this->readPosition + 1 < maxReadPos) && isValidDigit(this->command[this->readPosition + 1]))) {
				print_error("Space required between arithmetic operations", this->readPosition);
				return new Token(ERROR, 0, NULL);
			}
			// Digit
			else if (isValidDigit(cur) || ((cur == '-') && (this->readPosition + 1 < maxReadPos) && isValidDigit(this->command[this->readPosition + 1]))) {
				number[numberLen++] = cur;
				this->readPosition++;
				while (this->command[this->readPosition]) {
					cur = this->command[this->readPosition];
					if (isValidDigit(cur)) {
						number[numberLen++] = cur;
						this->readPosition++;
						continue;
					} else if (cur == 32 || cur == '(' || cur == ')') {
						number[numberLen] = '\0';
						int ret;
						if (number[0] == '-') {
							number[0] = '0';
							ret = -atoi(number);
						} else
							ret = atoi(number);
						return new Token(NUMBER, this->readPosition - 1, new TokenValue(INTEGER, &ret));
					}
					else if (isValidVarSymbol(cur)) {
						print_error("Invalid variable identifier - cannot start with a number", this->readPosition - numberLen);
						return new Token(ERROR, 0, NULL);
					}
					else {
						print_error("Invalid number [0-9]", this->readPosition);
						return new Token(ERROR, 0, NULL);
					}
				}
				print_error("Right parenthesis missing", this->readPosition);
				return new Token(ERROR, 0, NULL);
			}
			// Variable
			else if (isValidVarSymbol(cur)) {
				variableName[variableLen++] = cur;
				this->readPosition++;
				while (this->command[this->readPosition]) {
					cur = this->command[this->readPosition];
					if (isValidDigit(cur) || isValidVarSymbol(cur)) {
						variableName[variableLen++] = cur;
						this->readPosition++;
						continue;
					} else if (cur == 32 || cur == '(' || cur == ')' || cur == '.') {
						variableName[variableLen] = '\0';
						char *variableRet = (char*) malloc(strlen((const char*) variableName));
						strcpy(variableRet, (const char*) variableName);
						return new Token(VARIABLE, this->readPosition - 1, new TokenValue(STRING, variableRet));
					} else {
						print_error("Invalid variable identifier", this->readPosition);
						return new Token(ERROR, 0, NULL);
					}
				}
				print_error("Right parenthesis missing", this->readPosition);
				return new Token(ERROR, 0, NULL);
			}
			// Invalid Symbol
			else {
				print_error("Unknown symbol", this->readPosition);
				return new Token(ERROR, 0, NULL);
			}
		}
	}
	return NULL;
}

char* Scanner::getCommand() {
	return this->command;
}
