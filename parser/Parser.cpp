/*
 * Parser.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include "Parser.h"
#include "../defines.h"
#include "../error_handler/error_handling.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

using namespace std;

int indent = 0;

Parser::Parser(char *command) {
	this->syntaxTree = NULL;
	this->scanner = new Scanner(command);
	this->next = NULL;
	this->savedTokens = 0;
	this->curIndex = 0;
	this->scannerError = false;

	Token *token = this->scanner->nextToken();
	if (!token) this->scannerError = true;
	while (token) {
		if (token->type == ERROR) {
			this->scannerError = true;
			break;
		}
		this->next = (Token**) realloc(this->next, ++this->savedTokens * sizeof(Token*));
		this->next[this->savedTokens - 1] = token;
		token = this->scanner->nextToken();
	}
}

Parser::~Parser() {
	delete this->scanner;
	if (this->next)
		free(this->next);
}

bool Parser::parse() {
	if (this->scannerError) return false;
	InternalNode *root = new InternalNode(TERM);
	bool ret = this->Term(root);
	if (ret) {
		this->syntaxTree = new AST(root->children[0]);
	}
	else
		delete root;
	return ret;
}

bool Parser::terminal(TokenType type, InternalNode *node) {
	if (ERROR_FOUND) return false;
	if (curIndex  >= this->savedTokens)
		return false;
	bool ret = (next[curIndex++]->type == type) ? true : false;
	if (ret)
		node->addChild(new Leaf(new Token(next[curIndex - 1])));
	// Error checking.
	if (curIndex == this->savedTokens && next[curIndex - 1]->type != RIGHT_PAR)
		print_error("Right parenthesis missing", next[curIndex]->getPosition());

	if ((curIndex < this->savedTokens) && (!next[curIndex - 1]->canBeFollowedBy(next[curIndex]->type))) {
		char *errorMsg = (char*) malloc(MAX_ERROR_MESSAGE_LENGTH);
		strcpy(errorMsg, " ");
		errorMsg = strcat(errorMsg, next[curIndex]->typeToString(next[curIndex - 1]->type));
		errorMsg = strcat(errorMsg, " cannot be followed by ");
		errorMsg = strcat(errorMsg, next[curIndex]->typeToString(next[curIndex]->type));
		print_error(errorMsg, next[curIndex - 1]->getPosition());
	}

	if (curIndex - 1 > 0 && next[curIndex - 1]->type == VARIABLE && next[curIndex]->type == LAMBDA_DOT && next[curIndex - 2]->type != LAMBDA)
		print_error("\\ missing", next[curIndex - 2]->getPosition());
	return ret;
}

bool Parser::Term(InternalNode *node) {
	if (ERROR_FOUND) return false;
	int savedIndex = this->curIndex;
	InternalNode *term = new InternalNode(TERM);
	bool ret = (this->curIndex = savedIndex, Term_1(term)) || (this->curIndex = savedIndex, Term_2(term)) || (this->curIndex = savedIndex, Term_3(term));
	if (ret && !ERROR_FOUND) {
		node->addChild(term);
	}
	else
		delete term;
	ret = (ERROR_FOUND) ? false:ret;

	return ret;
}

bool Parser::Term_1(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(VARIABLE, node);
}

bool Parser::Term_2(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return Number(node);
}

bool Parser::Term_3(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(LEFT_PAR, node) && X(node);
}

bool Parser::Number(InternalNode *node) {
	if (ERROR_FOUND) return false;
	int savedIndex = this->curIndex;
	InternalNode *num = new InternalNode(NUMBER_EXP);
	bool ret = (this->curIndex = savedIndex, Number_1(num)) || (this->curIndex = savedIndex, Number_2(num));
	if (ret)
		node->addChild(num);
	else
		delete num;
	return ret;
}

bool Parser::Number_1(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(NUMBER, node);
}

bool Parser::Number_2(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(LEFT_PAR, node) && Number(node) && Z(node);
}

bool Parser::X(InternalNode *node) {
	if (ERROR_FOUND) return false;
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, X_1(node)) || (this->curIndex = savedIndex, X_2(node));
}

bool Parser::X_1(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return Term(node) && Y(node);
}

bool Parser::X_2(InternalNode *node) {
	if (ERROR_FOUND) return false;
	bool ret = terminal(LAMBDA, node) && terminal(VARIABLE, node) && terminal(LAMBDA_DOT, node) && Term(node);
	if (ret) {
		if (terminal(RIGHT_PAR, node))
 			return true;
		else
			print_error(") missing.", this->curIndex + 1);
	}
	return false;
}

bool Parser::Y(InternalNode *node) {
	if (ERROR_FOUND) return false;
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, Y_1(node)) || (this->curIndex = savedIndex, Y_2(node));
}

bool Parser::Y_1(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return Term(node) && terminal(RIGHT_PAR, node);
}

bool Parser::Y_2(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(RIGHT_PAR, node);
}

bool Parser::Z(InternalNode *node) {
	if (ERROR_FOUND) return false;
	int savedIndex = this->curIndex;
	return (this->curIndex = savedIndex, Z_1(node)) || (this->curIndex = savedIndex, Z_2(node));
}

bool Parser::Z_1(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(RIGHT_PAR, node);
}

bool Parser::Z_2(InternalNode *node) {
	if (ERROR_FOUND) return false;
	return terminal(OPERATOR, node) && Number(node) && terminal(RIGHT_PAR, node);
}

void Parser::postProcess() {
	this->syntaxTree->refine();
//	this->syntaxTree->doCalculations();
	this->syntaxTree->simplify();
}

void Parser::printSyntaxTree() {
	this->syntaxTree->print();
}
