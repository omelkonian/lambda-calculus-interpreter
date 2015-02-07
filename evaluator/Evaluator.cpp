/*
 * Evaluator.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#include "Evaluator.h"
#include <assert.h>
#include <stdio.h>

#include <iostream>

using namespace std;

Evaluator::Evaluator(AST *syntaxTree) {
	this->syntaxTree = syntaxTree;
}

Evaluator::~Evaluator() {
	delete this->syntaxTree;
}

char* Evaluator::evaluate() {
	bool betaExists = this->syntaxTree->bReductionExists();
	bool etaExists = this->syntaxTree->etaConversionExists();
	while (betaExists || etaExists) {
		if (betaExists) {
			// Execute leftmost application (lazy evaluation)
			InternalNode *nextApplication = this->syntaxTree->getFirstApplication();
			InternalNode *parent = this->syntaxTree->getParent(nextApplication);
			InternalNode *newNode = this->syntaxTree->substitute(nextApplication);

			if (!parent) {
				// Replace root.
				delete this->syntaxTree->getRoot();
				this->syntaxTree->setRoot(newNode);
			} else {
				for (int i = 0; i < (int) parent->children.size(); i++) {
					if (dynamic_cast<InternalNode*>(parent->children[i]) && ((InternalNode*) parent->children[i])->type == APPLICATION) {
						delete parent->children[i];
						parent->children[i] = newNode;
						break;
					}
					assert(i != (int ) (parent->children.size() - 1));
				}
			}
		}
		else if (etaExists) {
			InternalNode *nextEta = this->syntaxTree->getEtaNode();
			this->syntaxTree->eta_convert(nextEta);
		}

		betaExists = this->syntaxTree->bReductionExists();
		etaExists = this->syntaxTree->etaConversionExists();
		char *newCommand = this->syntaxTree->toCommand();
		cout << "-> " << newCommand << endl;
	}

	return this->syntaxTree->toCommand();
}