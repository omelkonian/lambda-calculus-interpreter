/*
 * InternalNode.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#include "InternalNode.h"
#include "Leaf.h"
#include "../token/Token.h"
#include "../token/TokenValue.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

InternalNode::InternalNode(NodeType type) {
	this->type = type;
}

InternalNode* InternalNode::getNewByCopy() {
	InternalNode *ret = NULL;
	this->copy(this, &ret);
	return ret;
}

InternalNode::~InternalNode() {
	for (int i = 0; i < (int) this->children.size(); i++)
		delete this->children[i];

	this->children.clear();
}

void InternalNode::addChild(Node* node) {
	this->children.push_back(node);
}

void InternalNode::print(int indent) {
	for (int i = 0; i < indent; i++)
		cout << " ";
	cout << this->getTypeAsString() << endl;
	for (int i = 0; i < (int) this->children.size(); i++)
		this->children[i]->print(indent + 3);
}

const char* InternalNode::getTypeAsString() {
	if (type == TERM)
		return "TERM";
	else if (type == APPLICATION)
		return "APPLICATION";
	else if (type == ABSTRACTION)
		return "ABSTRACTION";
	else if (type == VARIABLE_ID)
		return "VARIABLE_ID";
	else if (type == NUMBER_EXP)
		return "NUMBER_EXP";
	else
		return "UNKNOWN";
}

int InternalNode::doCalculations() {
	assert(this->type == NUMBER_EXP);

	if (this->children.size() == 1) {
		if (dynamic_cast<InternalNode*>(this->children[0]) != 0)
			return ((InternalNode*) this->children[0])->doCalculations();
		return ((Leaf*) this->children[0])->token->value->value.number;
	} else if (this->children.size() == 3)
		return ((InternalNode*) this->children[1])->doCalculations();
	else {
		int operand1 = ((InternalNode*) this->children[1])->doCalculations();
		int operand2 = ((InternalNode*) this->children[3])->doCalculations();
		char oper = ((Leaf*) this->children[2])->token->value->value.character;

		int result;
		switch (oper) {
		case '+':
			result = operand1 + operand2;
			break;
		case '-':
			result = operand1 - operand2;
			break;
		case '*':
			result = operand1 * operand2;
			break;
		case '/':
			result = operand1 / operand2;
			break;
		case '%':
			result = operand1 % operand2;
			break;
		}
		return result;
	}
}

void InternalNode::copy(Node* node, InternalNode **toInsert) {
	if (dynamic_cast<InternalNode*>(node)) {
		InternalNode *copy = (InternalNode*) node;

		char leftPar = '(';
		char rightPar = ')';

		InternalNode *newNode = new InternalNode(copy->type);
		if (copy->type == VARIABLE_ID) {
			char *varName = ((Leaf*) copy->children[0])->token->value->value.string;
			char *newVarName = (char*) malloc(strlen(varName));
			strcpy(newVarName, varName);
			newNode->children.push_back(new Leaf(new Token(VARIABLE, 0, new TokenValue(STRING, newVarName))));
		} else if (copy->type == NUMBER_EXP) {
			int num = ((Leaf*) copy->children[0])->token->value->value.number;
			newNode->children.push_back(new Leaf(new Token(NUMBER, 0, new TokenValue(INTEGER, &num))));
		} else if (copy->type == ABSTRACTION) {
			newNode->children.push_back(new Leaf(new Token(LEFT_PAR, 0, new TokenValue(CHAR, &leftPar))));
			InternalNode *n2 = NULL;
			char *varName = ((Leaf*) copy->children[1])->token->value->value.string;
			char *newVarName = (char*) malloc(strlen(varName));
			strcpy(newVarName, varName);
			newNode->children.push_back(new Leaf(new Token(VARIABLE, 0, new TokenValue(STRING, newVarName))));
			this->copy(copy->children[2], &n2);
			newNode->children.push_back(n2);
			newNode->children.push_back(new Leaf(new Token(RIGHT_PAR, 0, new TokenValue(CHAR, &rightPar))));
		} else if (copy->type == APPLICATION) {
			newNode->children.push_back(new Leaf(new Token(LEFT_PAR, 0, new TokenValue(CHAR, &leftPar))));
			InternalNode *n1 = NULL, *n2 = NULL;
			this->copy(copy->children[1], &n1);
			this->copy(copy->children[2], &n2);
			newNode->children.push_back(n1);
			newNode->children.push_back(n2);
			newNode->children.push_back(new Leaf(new Token(RIGHT_PAR, 0, new TokenValue(CHAR, &rightPar))));
		}
		*toInsert = newNode;
	}
}
