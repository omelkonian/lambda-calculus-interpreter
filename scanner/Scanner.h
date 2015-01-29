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

	Token* nextToken();
};

#endif /* LCI_SCANNER_SCANNER_H_ */
