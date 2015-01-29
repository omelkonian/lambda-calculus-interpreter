/*
 * Parser.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include "Parser.h"
#include <malloc.h>
#include <stdio.h>

#include <iostream>

using namespace std;

int indent = 0;

Parser::Parser(char *command) {
	this->scanner = new Scanner(command);
	this->next = NULL;
	this->savedTokens = 0;
	this->curIndex = 0;
	while (Token *token = this->scanner->nextToken()) {
		this->next = (Token**) realloc(this->next, ++this->savedTokens * sizeof(Token*));
		this->next[this->savedTokens - 1] = token;
	}
}

Parser::~Parser() {
	delete this->scanner;
	free(this->next);
}

bool Parser::parse() {
	return this->Term();
}

bool Parser::terminal(TokenType type) {
	return (next[curIndex++]->type == type) ? true : false;
}

bool Parser::Term() {
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, Term_1()) || (this->curIndex = savedIndex, Term_2()) || (this->curIndex = savedIndex, Term_3());
}

bool Parser::Term_1() {
	return terminal(VARIABLE);
}

bool Parser::Term_2() {
	return Number();
}

bool Parser::Term_3() {
	return terminal(LEFT_PAR) && X();
}

bool Parser::Number() {
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, Number_1()) || (this->curIndex = savedIndex, Number_2());
}

bool Parser::Number_1() {
	return terminal(NUMBER);
}

bool Parser::Number_2() {
	return terminal(LEFT_PAR) && Number() && Z();
}

bool Parser::X() {
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, X_1()) || (this->curIndex = savedIndex, X_2());
}

bool Parser::X_1() {
	return Term() && Y();
}

bool Parser::X_2() {
	return terminal(LAMBDA) && terminal(VARIABLE) && terminal(LAMBDA_DOT) && Term() && terminal(RIGHT_PAR);
}

bool Parser::Y() {
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, Y_1()) || (this->curIndex = savedIndex, Y_2());
}

bool Parser::Y_1() {
	return Term() && terminal(RIGHT_PAR);
}

bool Parser::Y_2() {
	return terminal(RIGHT_PAR);
}

bool Parser::Z() {
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, Z_1()) || (this->curIndex = savedIndex, Z_2());
}

bool Parser::Z_1() {
	return terminal(RIGHT_PAR);
}

bool Parser::Z_2() {
	return terminal(OPERATOR) && Number() && terminal(RIGHT_PAR);
}
