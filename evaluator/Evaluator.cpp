/*
 * Evaluator.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#include "Evaluator.h"
#include "../scanner/Scanner.h"
#include "../defines.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
	while (betaExists || etaExists) {
		if (betaExists) {
			cout << "BETA!" << endl;
			// Check if result is list
			char *newCommand = this->syntaxTree->toCommand();
			string command(newCommand);
			free(newCommand);
			command = this->aliasManager->deTranslate(command);
			cout << "Is " << command << " a list?" << endl;

			if (this->isList(command)) {
				cout << "IS A LIST" << endl;
				LIST_RESULT = true;
				break;
			}
			cout << "Nope" << endl;

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
		} else if (etaExists) {
			cout << "ETA!" << endl;
			InternalNode *nextEta = this->syntaxTree->getEtaNode();
			this->syntaxTree->eta_convert(nextEta);
		}

		betaExists = this->syntaxTree->bReductionExists();
		etaExists = this->syntaxTree->etaConversionExists();

		if (TRACE) {
			char *newCommand = this->syntaxTree->toCommand();
			cerr << "\33[0;32m" << "->" << "\33[0m";
			string command(newCommand);
			cout << this->aliasManager->deTranslate(command) << endl;
			free(newCommand);
		}
	}

	return this->syntaxTree->toCommand();
}

bool Evaluator::isList(string term) {
	int index = 0;
	string tmp;
	if (term.compare("nil") == 0)
		return true;
	while (index < (int) term.size()) {
		// 1. Get "((cons "
		tmp = term.substr(index, 7);
		if (!((index < (int) term.size()) && (tmp.compare("((cons ") == 0)))
			return false;
		index += 7;
		// 2. Get nested term
		while (term[index] == 32) {
			index++;
			if (index >= (int) term.size()) return false;
		}
		if (term[index] == '(') {
			int parCount = 1;
			index++;
			while (true) {
				if (index >= (int) term.size()) return false;
				if (term[index] == ')') {
					if (--parCount == 0) break;
				}
				if (term[index] == '(') parCount++;
				index++;
			}
		}
		else {
			while (Scanner::isValidVarSymbol(term[index]) || Scanner::isValidDigit(term[index])) {
				index++;
				if (index >= (int) term.size()) return false;
			}
		}
		// 3. Get ") "
		if (term[index++] != ')') return false;
		if (index >= (int) term.size()) return false;
		index++;
		// 4. break at nil
		tmp = term.substr(index, 4);
		if (tmp.compare(" nil") == 0) return true;

		index++;
	}
	return false;
}
