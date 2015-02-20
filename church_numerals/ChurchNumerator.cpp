/*
 * ChurchNumerator.cpp
 *
 *  Created on: 7 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#include "ChurchNumerator.h"
#include "../parser/Parser.h"
#include "../abstract_syntax_tree/InternalNode.h"
#include <string>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
using namespace std;

ChurchNumerator::ChurchNumerator(AST *syntaxTree, AliasManager *aliasManager) {
	this->syntaxTree = syntaxTree;
	this->aliasManager = aliasManager;
}

ChurchNumerator::~ChurchNumerator() {
}

void ChurchNumerator::enchurch() {
	vector<InternalNode*> numberExpressions = this->syntaxTree->getAllNumberExpressions();
	for (int i = (int) numberExpressions.size() - 1; i >= 0 ; i--) {
		InternalNode *cur = numberExpressions[i];
		if (cur == NULL)
			continue;

		// #1. (Num_Exp)
		if (cur->children.size() == 3) {
			continue;
		}

		// #2. Number
		if (cur->children.size() == 1) {
			// Get number
			int number = ((Leaf*) cur->children[0])->token->value->value.number;
			// Construct numeral
			InternalNode *numeral = this->constructChurchNumeral(number);
			// Replace in syntaxTree
			this->syntaxTree->replace(cur, numeral);
		}
		// #3. (Num_Exp Operator Num_Exp)
		else if (cur->children.size() == 5) {
			InternalNode *numExp1 = ((InternalNode*) cur->children[1])->getNewByCopy();
			InternalNode *numExp2 = ((InternalNode*) cur->children[3])->getNewByCopy();
			assert(dynamic_cast<Leaf*>(cur->children[2]));
			Leaf *oper = (Leaf*) cur->children[2];
			string operName;
			switch (oper->token->value->value.character) {
			case '+':
				operName = "add";
				break;
			case '-':
				operName = "sub";
				break;
			case '*':
				operName = "mul";
				break;
			case '^':
				operName = "exp";
				break;
			}
			string operTerm = this->aliasManager->deAlias(operName);
			InternalNode *operNode = this->termToNode(operTerm);

			InternalNode *replacement = new InternalNode(APPLICATION);
			replacement->children.push_back(new Leaf(new Token(LEFT_PAR, 0, NULL)));

			InternalNode *application1 = new InternalNode(APPLICATION);
			application1->children.push_back(new Leaf(new Token(LEFT_PAR, 0, NULL)));
			application1->children.push_back(operNode);
			application1->children.push_back(numExp1);
			application1->children.push_back(new Leaf(new Token(RIGHT_PAR, 0, NULL)));

			replacement->children.push_back(application1);
			replacement->children.push_back(numExp2);

			replacement->children.push_back(new Leaf(new Token(RIGHT_PAR, 0, NULL)));

			this->syntaxTree->replace(cur, replacement);
		}
		else
			cout << "INVALID NUMBER OF CHILDREN IN NUM_EXP" << endl;

	}
}

void ChurchNumerator::dechurch() {
	vector<pair<InternalNode*, int> > churchNumerals = this->syntaxTree->getAllChurchNumerals();
	for (int i = 0; i < (int) churchNumerals.size(); i++) {
		InternalNode *newSubtree = new InternalNode(NUMBER_EXP);
		int number = churchNumerals[i].second;
		newSubtree->children.push_back(new Leaf(new Token(NUMBER, 0, new TokenValue(INTEGER, &number))));

		// Replace in syntaxTree
		this->syntaxTree->replace(churchNumerals[i].first, newSubtree);
	}
}

void ChurchNumerator::printTree() {
	this->syntaxTree->print();
}

InternalNode* ChurchNumerator::termToNode(string term) {
	char *command = (char*) malloc(strlen(term.c_str()) + 1);
	strcpy(command, term.c_str());

	Parser *parser = new Parser(command);
	assert(parser->parse());
	parser->postProcess();
	InternalNode *ret = (InternalNode*) parser->syntaxTree->getRoot();
	parser->syntaxTree->setRoot(NULL);
	free(command);
	delete parser;

	return ret;
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

	return this->termToNode(numeral);
}
