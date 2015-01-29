/*
 * Leaf.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#include "Leaf.h"

Leaf::Leaf(Token* token) {
	this->token = token;
}

Leaf::~Leaf() {
	delete this->token;
}

void Leaf::print() {
	this->token->print();
}
