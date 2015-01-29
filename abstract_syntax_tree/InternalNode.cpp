/*
 * InternalNode.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#include "InternalNode.h"
#include <iostream>

using namespace std;

InternalNode::InternalNode(NodeType type) {
	this->type = type;
//	if (type == 0)
//		this->type = TERM;
//	else if (type == 1)
//		this->type = APPLICATION;
//	else
//		this->type = ABSTRACTION;
}

InternalNode::~InternalNode() {
	for (int i = 0; i < (int) this->children.size(); i++)
		delete this->children[i];
	this->children.clear();
}

void InternalNode::addChild(Node* node) {
	this->children.push_back(node);
}

void InternalNode::print() {
	cout << "Type: " << this->getTypeAsString() << endl;
	for (int i = 0; i < (int) this->children.size(); i++)
		this->children[i]->print();
}

const char* InternalNode::getTypeAsString() {
	if (type == 0)
		return "TERM";
	else if (type == 1)
		return "APPLICATION";
	else
		return "ABSTRACTION";
}
