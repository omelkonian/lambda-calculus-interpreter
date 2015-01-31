/*
 * InternalNode.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#include "InternalNode.h"
#include "Leaf.h"
#include <iostream>
#include <assert.h>

using namespace std;

InternalNode::InternalNode(NodeType type) {
	this->type = type;
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
			return ((InternalNode*)this->children[0])->doCalculations();
		return ((Leaf*) this->children[0])->token->value->value.number;
	}
	else if (this->children.size() == 3)
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
