/*
 * Scanner.h
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#ifndef LCI_SCANNER_SCANNER_H_
#define LCI_SCANNER_SCANNER_H_

#include "../token/Token.h"

/**
 * INPUT: A string representing the command given.
 * OUTPUT: A stream of token accessed via nextToken().
 */
class Scanner {
	char *command;
	int readPosition;

	bool firstSymbolRead;
public:
	Scanner(char *command);
	virtual ~Scanner();

	char* getCommand();

	Token* nextToken();

	static bool isValidVarSymbol(char symbol);
	static bool isValidDigit(char symbol);
	static bool isValidOperator(char symbol);

};

#endif /* LCI_SCANNER_SCANNER_H_ */
