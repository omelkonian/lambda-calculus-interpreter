/*
 * Token.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include "Token.h"
#include <iostream>

using namespace std;

const char* typeToString(TokenType type) {
	switch (type) {
	case 0:
		return "LAMBDA";
	case 1:
		return "LAMBDA_DOT";
	case 2:
		return "LEFT_PAR";
	case 3:
		return "RIGHT_PAR";
	case 4:
		return "VARIABLE";
	case 5:
		return "NUMBER";
	case 6:
		return "OPERATOR";
	default:
		return "UNKNOWN";
	}
}

Token::Token(TokenType type, int position, TokenValue *value) {
	this->type = type;
	this->position = position;
	this->value = value;
}

Token::~Token() {
	delete this->value;
}

void Token::print() {
	cout << endl;
	cout << "Type: " << typeToString(this->type) << endl;
	this->value->print();
	cout << "Position: " << this->position << endl;
	cout << endl;
}

void Token::print(int indent) {
	for (int i = 0; i < indent; i++)
		cout << " ";
	if (this->type == VARIABLE || this->type == NUMBER || this->type == OPERATOR) {
		cout << typeToString(this->type) << ":  ";
		this->value->print(0);
	}
	else
		cout << typeToString(this->type) << endl;
}
