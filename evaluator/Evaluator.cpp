/*
 * Evaluator.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#include "Evaluator.h"
#include <assert.h>

#include <iostream>

using namespace std;

Evaluator::Evaluator(AST *syntaxTree) {
	this->syntaxTree = syntaxTree;
}

Evaluator::~Evaluator() {
}

char* Evaluator::evaluate() {
	while (this->syntaxTree->bReductionExists()) {
		// Execute leftmost application (lazy evaluation)
		InternalNode *parent = this->syntaxTree->getFirstApplicationParent();
		InternalNode *nextApplication = this->syntaxTree->getFirstApplication();

		InternalNode *newNode = this->syntaxTree->substitute(nextApplication);

		if (!parent || parent == this->syntaxTree->getRoot()) {
			// Replace root.
			delete this->syntaxTree->getRoot();
			this->syntaxTree->setRoot(newNode);
		}
		else {
			for (int i = 0; i < (int) parent->children.size(); i++) {
				if (dynamic_cast<InternalNode*>(parent->children[i]) && ((InternalNode*)parent->children[i])->type == APPLICATION) {
					delete parent->children[i];
					parent->children[i] = newNode;
					break;
				}
				assert(i != (int) (parent->children.size() - 1));
			}
		}

		char *newCommand = this->syntaxTree->toCommand();
		cout << "-> " << newCommand << endl;
	}
	return this->syntaxTree->toCommand();
}
