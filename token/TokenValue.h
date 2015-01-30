/*
 * TokenValue.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#ifndef TOKEN_TOKENVALUE_H_
#define TOKEN_TOKENVALUE_H_

enum ValueType {
	STRING,
	CHAR,
	INTEGER,
	NONE
};

class TokenValue {
public:
	ValueType type;

	union value_t {
		char *string;
		char character;
		int number;
	} value;

	TokenValue(TokenValue *copy);
	TokenValue(ValueType type, void *val);
	virtual ~TokenValue();

	void print();
	void print(int indent);
};

#endif /* TOKEN_TOKENVALUE_H_ */
