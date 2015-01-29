/*
 * Token.h
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#ifndef LCI_SCANNER_TOKEN_H_
#define LCI_SCANNER_TOKEN_H_

#include <string>
#include "TokenValue.h"

using namespace std;

enum TokenType {
	LAMBDA,
	LAMBDA_DOT,
	LEFT_PAR,
	RIGHT_PAR,
	VARIABLE,
	NUMBER,
	OPERATOR
};

class Token {
	int position;
	TokenValue *value;
public:
	TokenType type;

	Token(TokenType type, int position, TokenValue *value);
	virtual ~Token();

	void print();
	void print(int indent);
};

#endif /* LCI_SCANNER_TOKEN_H_ */
