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

enum TokenType {
	LAMBDA,
	LAMBDA_DOT,
	LEFT_PAR,
	RIGHT_PAR,
	VARIABLE,
	NUMBER,
	OPERATOR,
	ERROR
};

class Token {
	int position;
public:
	TokenValue *value;
	TokenType type;

	Token(Token *copy);
	Token(TokenType type, int position, TokenValue *value);
	virtual ~Token();

	int getPosition();

	bool canBeFollowedBy(TokenType type);

	static const char* typeToString(TokenType type);

	void print();
	void print(int indent);
};

#endif /* LCI_SCANNER_TOKEN_H_ */
