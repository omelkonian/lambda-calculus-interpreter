/*
 * TokenValue.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#ifndef TOKEN_TOKENVALUE_H_
#define TOKEN_TOKENVALUE_H_

class TokenValue {
public:
	int type; //0: string, 1: char, 2: int

	union value_t {
		char *string;
		char character;
		int number;
	} value;


	TokenValue(int type, void *val);
	virtual ~TokenValue();

	void print();
};

#endif /* TOKEN_TOKENVALUE_H_ */
