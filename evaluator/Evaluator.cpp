/*
 * Evaluator.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#include "Evaluator.h"
#include "../scanner/Scanner.h"
#include "../error_handler/AutoCorrector.h"
#include "../church_numerals/ChurchNumerator.h"
#include "../defines.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

using namespace std;

Evaluator::Evaluator(AST *syntaxTree, AliasManager *aliasManager) {
	this->syntaxTree = syntaxTree;
	this->aliasManager = aliasManager;
}

Evaluator::~Evaluator() {
	delete this->syntaxTree;
}

char* Evaluator::evaluate() {
	bool betaExists = this->syntaxTree->bReductionExists();
	bool etaExists = this->syntaxTree->etaConversionExists();

	string previous("$$$$$$$");

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
					if (dynamic_cast<InternalNode*>(parent->children[i]) && ((InternalNode*) parent->children[i]) == nextApplication) {
						delete parent->children[i];
						parent->children[i] = newNode;
						break;
					}
					assert(i != (int ) (parent->children.size() - 1));
				}
			}
		} else if (etaExists) {
			InternalNode *nextEta = this->syntaxTree->getEtaNode();
			this->syntaxTree->eta_convert(nextEta);
		}

		betaExists = this->syntaxTree->bReductionExists();
		etaExists = this->syntaxTree->etaConversionExists();

		char *newCommand = this->syntaxTree->toCommand();
		string command(newCommand);
		free(newCommand);

		// Infinite term detection.
		if (command.compare(previous) == 0 || (command.find(previous) == 0)) {
			cerr << "\33[0;1;31m" << "ERROR" << "\33[0m" << " Infinite term detected: (" << command << ")" << endl;
			ERROR_FOUND = true;
			break;
		}

		if (TRACE) {
			cerr << "\33[0;32m" << "->" << "\33[0m";
			cout << this->aliasManager->deTranslate(command) << endl;

			if (!NON_STOP) {
				cout << "?- ";
				cout << flush;
				string command;
				cin >> command;
				while (command.compare("step") != 0 && command.compare("run") != 0 && command.compare("abort") != 0) {
					cout << "?- ";
					cout << flush;
					cin >> command;
				}
				if (command.compare("run") == 0)
					NON_STOP = true;
				else if (command.compare("abort") == 0) {
					cout << "EXECUTION ABORTED!" << endl;
					break;
				}
			}
		}
		previous = command;
	}

	return this->syntaxTree->toCommand();
}
