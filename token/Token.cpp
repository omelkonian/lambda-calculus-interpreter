/*
 * Token.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include "Token.h"
#include <iostream>
#include <vector>

using namespace std;

// Syntactic sugar for vector initialization.
template <class T> class vector_inserter{
public:
    std::vector<T>& v;
    vector_inserter(std::vector<T>& v):v(v){}
    vector_inserter& operator,(const T& val){v.push_back(val);return *this;}
};
template <class T> vector_inserter<T>& operator+=(std::vector<T>& v,const T& x){
    return vector_inserter<T>(v),x;
}

const char* Token::typeToString(TokenType type) {
	switch (type) {
	case 0:
		return "\\";
	case 1:
		return ".";
	case 2:
		return "(";
	case 3:
		return ")";
	case 4:
		return "variable";
	case 5:
		return "number";
	case 6:
		return "operator";
	default:
		return "UNKNOWN";
	}
}

Token::Token(Token* copy) {
	this->type = copy->type;
	this->position = copy->position;
	this->value = new TokenValue(copy->value);
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

bool Token::canBeFollowedBy(TokenType type) {
	std::vector<TokenType> validTypes;
	switch (this->type) {
		case LAMBDA:
			validTypes += VARIABLE;
			break;
		case LAMBDA_DOT:
			validTypes += VARIABLE, NUMBER, LEFT_PAR;
			break;
		case LEFT_PAR:
			validTypes += LEFT_PAR, NUMBER, LAMBDA, VARIABLE;
			break;
		case RIGHT_PAR:
			validTypes += OPERATOR, RIGHT_PAR, VARIABLE, LEFT_PAR, NUMBER;
			break;
		case VARIABLE:
			validTypes += VARIABLE, LEFT_PAR, RIGHT_PAR, NUMBER, LAMBDA_DOT;
			break;
		case NUMBER:
			validTypes += VARIABLE, OPERATOR, LEFT_PAR, RIGHT_PAR, NUMBER;
			break;
		case OPERATOR:
			validTypes += NUMBER, LEFT_PAR;
			break;
	}
	bool found = false;
	for (int i = 0; i < (int)validTypes.size(); i++) {
		if (validTypes[i] == type)
			found = true;
	}
	return found;
}

int Token::getPosition() {
	return this->position;
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
