/*
 * AST.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#include "AST.h"
#include "InternalNode.h"
#include "Leaf.h"
#include "../defines.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

AST::AST(Node *root) {
	this->root = root;
}

AST::~AST() {
	delete this->root;
}

Node* AST::getRoot() {
	return this->root;
}

void AST::print() {
	this->root->print(0);
}

char* AST::toCommand() {
	char *command = (char*) malloc(MAX_COMMAND_LENGTH);
	int writePosition = 0;
	this->toCommand1(this->root, &writePosition, command);
	return command;
}

void AST::toCommand1(Node* node, int (*writePos), char *command) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == ABSTRACTION) {
			command[((*writePos))++] = '(';
			command[(*writePos)++] = '\\';

			char *var = ((Leaf*) cur->children[1])->token->value->value.string;
			for (int i = 0; i < (int) strlen(var); i++)
				command[(*writePos)++] = var[i];

			command[(*writePos)++] = '.';
			command[(*writePos)++] = ' ';
			this->toCommand1(cur->children[2], writePos, command);
			command[(*writePos)++] = ')';
		} else if (cur->type == APPLICATION) {
			command[(*writePos)++] = '(';
			this->toCommand1(cur->children[1], writePos, command);
			command[(*writePos)++] = ' ';
			this->toCommand1(cur->children[2], writePos, command);
			command[(*writePos)++] = ')';
		} else if (cur->type == VARIABLE_ID) {
			char *var = ((Leaf*) cur->children[0])->token->value->value.string;
			for (int i = 0; i < (int) strlen(var); i++)
				command[(*writePos)++] = var[i];
		} else if (cur->type == NUMBER_EXP) {
			char *num = (char*) malloc(MAX_NUMBER_DIGITS);
			sprintf(num, "%d", ((Leaf*) cur->children[0])->token->value->value.number);
			for (int i = 0; i < (int) strlen(num); i++)
				command[(*writePos)++] = num[i];
			free(num);
		} else { // General term
			command[((*writePos))++] = '(';
			this->toCommand1(cur->children[0], writePos, command);
			command[(*writePos)++] = ')';
		}
	}
}

void AST::refine() {
	this->refine1(this->root);
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
				if (dynamic_cast<Leaf*>(cur->children[0]) && (((Leaf*) cur->children[0])->token->type == VARIABLE))
					cur->type = VARIABLE_ID;
			} else
				cur->type = NUMBER_EXP;
		}
		for (int i = 0; i < (int) cur->children.size(); i++)
			this->refine1(cur->children[i]);
	}
}

void AST::doCalculations() {
	this->doCalculations1(this->root);
}

void AST::doCalculations1(Node* node) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == NUMBER_EXP) {
			int result = cur->doCalculations();
			// Delete all children.
			for (int i = 0; i < (int) cur->children.size(); i++)
				delete cur->children[i];
			cur->children.clear();
			cur->children.push_back(new Leaf(new Token(NUMBER, 0, new TokenValue(INTEGER, &result))));
		} else {
			for (int i = 0; i < (int) cur->children.size(); i++)
				this->doCalculations1(cur->children[i]);
		}
	}
}

void AST::simplify() {
	this->simplify1(this->root);
}

void AST::simplify1(Node* node) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == ABSTRACTION) {
			// Delete 1st('\') and 3rd('.') children.
			Node *n1 = cur->children.at(1);
			Node *n2 = cur->children.at(3);
			cur->children.erase(cur->children.begin() + 1);
			cur->children.erase(cur->children.begin() + 2);
			delete n1;
			delete n2;
		}
		for (int i = 0; i < (int) cur->children.size(); i++)
			this->simplify1(cur->children[i]);
	}
}
