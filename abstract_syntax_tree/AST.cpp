/*
 * AST.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#include "AST.h"
#include "InternalNode.h"
#include "Leaf.h"

#include <iostream>

using namespace std;

AST::AST(Node *root) {
	this->root = root;
}

AST::~AST() {
	delete this->root;
}

void AST::print() {
	this->root->print(0);
}

void AST::refine() {
	this->refine1(this->root);
//	((InternalNode*)this->root)->type = TERM;
}

void AST::refine1(Node* node) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == TERM) {
			if (cur->children.size() == 4)
				cur->type = APPLICATION;
			else if (cur->children.size() == 6)
				cur->type = ABSTRACTION;
			else if (cur->children.size() == 1) {
				if (dynamic_cast<Leaf*>(cur->children[0]) && (((Leaf*)cur->children[0])->token->type == VARIABLE))
					cur->type = VARIABLE_ID;
			}
			else
				cur->type = NUMBER_EXP;
		}
		for (int i = 0; i < (int) cur->children.size(); i++)
			this->refine1(cur->children[i]);
	}
}

void AST::doCalculations() {
	this->doCalculations1(this->root);
}

Node* AST::getRoot() {
	return this->root;
}

void AST::doCalculations1(Node* node) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == NUMBER_EXP) {
			int result = cur->doCalculations();
			// Delete all children.
			for (int i = 0; i < (int)cur->children.size(); i++)
				delete cur->children[i];
			cur->children.clear();
			cur->children.push_back(new Leaf(new Token(NUMBER, 0, new TokenValue(INTEGER, &result))));
		}
		else {
			for (int i = 0; i < (int) cur->children.size(); i++)
				this->doCalculations1(cur->children[i]);
		}
	}
}
