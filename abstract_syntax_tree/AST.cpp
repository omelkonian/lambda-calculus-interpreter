/*
 * AST.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#include "AST.h"

AST::AST(Node *root) {
	this->root = root;
}

AST::~AST() {
	delete this->root;
}

void AST::print() {
	this->root->print(0);
}
