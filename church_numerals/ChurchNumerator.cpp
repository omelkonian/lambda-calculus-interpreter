/*
 * ChurchNumerator.cpp
 *
 *  Created on: 7 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#include "ChurchNumerator.h"
#include "../parser/Parser.h"
#include <string>
#include <stdlib.h>
#include <string.h>

#include <iostream>
using namespace std;

ChurchNumerator::ChurchNumerator(AST *syntaxTree) {
	this->syntaxTree = syntaxTree;
}

ChurchNumerator::~ChurchNumerator() {
}

void ChurchNumerator::termsToNumbers() {
	vector<InternalNode*> numberExpressions = this->syntaxTree->getAllNumberExpressions();
	for (int i = 0; i < (int)numberExpressions.size(); i++) {
		// Get number
		int number = ((Leaf*)numberExpressions[i]->children[0])->token->value->value.number;

		// Construct numeral
		InternalNode *numeral = this->constructChurchNumeral(number); // TODO negative numbers

		// Replace in syntaxTree
		this->syntaxTree->replace(numberExpressions[i], numeral);
	}
}

InternalNode* ChurchNumerator::constructChurchNumeral(int number) {
	string numeral = "(\\f. (\\x. ))";
	for (int i = 0; i < number; i++) {
		numeral.insert(10, ")");
	}
	numeral.insert(10, "x");
	for (int i = 0; i < number; i++) {
		numeral.insert(10, "(f ");
	}

	char *command = (char*) malloc(strlen(numeral.c_str()) + 1);
	strcpy(command, numeral.c_str());

	Parser *parser = new Parser(command);
	parser->parse();
	parser->postProcess();
	return (InternalNode*) parser->syntaxTree->getRoot();
}
