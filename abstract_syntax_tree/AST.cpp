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
#include <assert.h>
#include <vector>

#include <iostream>

using namespace std;

AST::AST(Node *root) {
	this->root = root;
	this->parent = NULL;
	this->varPool = new VariablePool();
}

AST::~AST() {
	delete this->root;
}

Node* AST::getRoot() {
	return this->root;
}

void AST::setRoot(Node* root) {
	this->root = root;
}

InternalNode* AST::getParent(Node* child) {
	InternalNode *parent = NULL;
	this->getParent1(this->root, child, &parent);
	return parent;
}

void AST::getParent1(Node *cur, Node* child, InternalNode** parent) {
	if (dynamic_cast<InternalNode*>(cur)) {
		InternalNode *curr = (InternalNode*) cur;
		for (int i = 0; i < (int) curr->children.size(); i++) {
			if (curr->children[i] == child) {
				*parent = curr;
				return;
			}
		}
		for (int i = 0; i < (int) curr->children.size(); i++) {
			if (*parent)
				break;
			this->getParent1(curr->children[i], child, parent);
		}
	}
}

void AST::print() {
	this->root->print(0);
}

char* AST::toCommand() {
	char *command = (char*) malloc(MAX_COMMAND_LENGTH);
	int writePosition = 0;
	this->toCommand1(this->root, &writePosition, command);
	command[writePosition] = '\0';
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
			else if (cur->children.size() == 1 && dynamic_cast<Leaf*>(cur->children[0]) && (((Leaf*) cur->children[0])->token->type == VARIABLE))
				cur->type = VARIABLE_ID;
			else if (cur->children.size() == 3 && dynamic_cast<InternalNode*>(cur->children[1]) && (((Leaf*) ((InternalNode*) cur->children[1])->children[0])->token->type == VARIABLE)) {
				cur->type = VARIABLE_ID;
				char *varName = ((Leaf*) ((InternalNode*) cur->children[1])->children[0])->token->value->value.string;
				char *newVarName = (char*) malloc(strlen(varName));
				strcpy(newVarName, varName);

				for (int i = 0; i < (int) cur->children.size(); i++)
					delete cur->children[i];
				cur->children.clear();

				cur->children.push_back(new Leaf(new Token(VARIABLE, 0, new TokenValue(STRING, newVarName))));
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

bool AST::bReductionExists() {
	bool found = false;
	this->bReductionExists1(this->root, &found);
	return found;
}

void AST::bReductionExists1(Node* node, bool* found) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == APPLICATION && ((InternalNode*) cur->children[1])->type == ABSTRACTION) {
			*found = true;
			return;
		}

		for (int i = 0; i < (int) cur->children.size(); i++)
			this->bReductionExists1(cur->children[i], found);
	}
}

bool AST::etaConversionExists() {
	bool found = false;
	this->etaConversionExists1(this->root, &found);
	return found;
}

void AST::etaConversionExists1(Node* node, bool* found) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == ABSTRACTION && ((InternalNode*) cur->children[2])->type == APPLICATION) {
			char *mainVar = ((Leaf*) cur->children[1])->token->value->value.string;
			InternalNode *application = (InternalNode*) cur->children[2];
			InternalNode *secondChild = (InternalNode*) application->children[2];
			if (secondChild->type == VARIABLE_ID && strcmp((((Leaf*) secondChild->children[0])->token->value->value.string), mainVar) == 0) {
				bool unbound = true;
				std::vector<char*> freeVariables = this->freeVariables((InternalNode*) application->children[1]);
				for (int i = 0; i < (int) freeVariables.size(); i++) {
					if (strcmp(freeVariables[i], mainVar) == 0)
						unbound = false;
				}
				if (unbound) {
					*found = true;
					return;
				}
			}
		}

		for (int i = 0; i < (int) cur->children.size(); i++)
			this->etaConversionExists1(cur->children[i], found);
	}
}

InternalNode* AST::substitute(InternalNode *node) {
	assert(node->type == APPLICATION);

	InternalNode *toInsert = (InternalNode*) node->children[2];
	InternalNode *insertTo = ((InternalNode*) node->children[1]);

	this->substitute1((InternalNode*) insertTo->children[2], toInsert, ((Leaf*) insertTo->children[1])->token->value->value.string);

	InternalNode *abstraction = (InternalNode*) node->children[1];
	InternalNode *toCopy = (InternalNode*) abstraction->children[2];
	InternalNode *newNode = toCopy->getNewByCopy();

	delete node->children[2];
	delete ((InternalNode*) node->children[1])->children[0];
	delete ((InternalNode*) node->children[1])->children[1];
	delete ((InternalNode*) node->children[1])->children[3];

	node->children.erase(node->children.begin() + 2);
	node->children.erase(node->children.begin() + 1);

	return newNode;
}

void AST::substitute1(InternalNode *node, InternalNode *toInsert, char *varToSub) {
	if (node->type == VARIABLE_ID && (strcmp(((Leaf*) node->children[0])->token->value->value.string, varToSub) == 0)) {
		for (int i = 0; i < (int) node->children.size(); i++)
			delete node->children[i];
		node->type = toInsert->type;
		node->children = toInsert->children;
	} else if (node->type == APPLICATION) {
		this->substitute1(((InternalNode*) node->children[1]), toInsert, varToSub);
		this->substitute1(((InternalNode*) node->children[2]), toInsert, varToSub);
	} else if (node->type == ABSTRACTION) {
		char *mainVar = ((Leaf*) node->children[1])->token->value->value.string;

		// #1. (\x. P)[x := N] -> \x.P
		if (strcmp(mainVar, varToSub) == 0) {
			toInsert = (InternalNode*) node->children[2];
			return;
		} else {
			char *x = varToSub;
			char *y = mainVar;

			std::vector<char*> fv_N = this->freeVariables(toInsert);
			std::vector<char*> fv_P = this->freeVariables((InternalNode*) node->children[2]);

			bool yInFV_N = false;
			bool xInFV_P = false;

			for (int i = 0; i < (int) fv_N.size(); i++) {
				if (strcmp(fv_N[i], y) == 0)
					yInFV_N = true;
			}
			for (int i = 0; i < (int) fv_P.size(); i++) {
				if (strcmp(fv_P[i], x) == 0)
					xInFV_P = true;
			}

			// #2 (\y. P)[x := N] -> \y. P[x := N]
			if (!xInFV_P || !yInFV_N)
				this->substitute1((InternalNode*) node->children[2], toInsert, varToSub);
			// #3 (\y. P)[x := N] -> \z. P[y := z][x := N]
			else {
				this->alpha_convert(node);
				this->substitute1((InternalNode*) node->children[2], toInsert, varToSub);
			}
		}
	}
}

void AST::eta_convert(InternalNode* application) {
	InternalNode *parent = this->getParent(application);
	InternalNode *applicationChild = (InternalNode*) application->children[2];
	InternalNode *newNode = ((InternalNode*) applicationChild->children[1])->getNewByCopy();
	if (parent) {
		for (int i = 0; i < (int) parent->children.size(); i++) {
			if (parent->children[i] == application) {
				delete parent->children[i];
				parent->children[i] = newNode;
			}
		}
	} else {
		// Delete root.
		delete this->root;
		this->root = newNode;
	}
}

InternalNode* AST::getFirstApplication() {
	InternalNode *found = NULL;
	this->getFirstApplication1(this->root, &found);
	return found;
}

void AST::getFirstApplication1(Node* node, InternalNode **found) {
	if (dynamic_cast<InternalNode*>(node)) {
		InternalNode *cur = ((InternalNode*) node);
		if (cur->type == APPLICATION && ((InternalNode*) cur->children[1])->type == ABSTRACTION)
			*found = cur;
		else {
			for (int i = 0; i < (int) cur->children.size(); i++) {
				if (*found)
					break;
				this->getFirstApplication1(cur->children[i], found);
			}
		}
		return;
	}
}

InternalNode* AST::getEtaNode() {
	InternalNode *found = NULL;
	this->getEtaNode1(this->root, &found);
	return found;
}

void AST::getEtaNode1(Node* node, InternalNode** found) {
	if (dynamic_cast<InternalNode*>(node) != 0) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == ABSTRACTION && ((InternalNode*) cur->children[2])->type == APPLICATION) {
			char *mainVar = ((Leaf*) cur->children[1])->token->value->value.string;
			InternalNode *application = (InternalNode*) cur->children[2];
			InternalNode *secondChild = (InternalNode*) application->children[2];
			if (secondChild->type == VARIABLE_ID && strcmp((((Leaf*) secondChild->children[0])->token->value->value.string), mainVar) == 0) {
				bool unbound = true;
				std::vector<char*> freeVariables = this->freeVariables((InternalNode*) application->children[1]);
				for (int i = 0; i < (int) freeVariables.size(); i++) {
					if (strcmp(freeVariables[i], mainVar) == 0)
						unbound = false;
				}
				if (unbound) {
					*found = cur;
					return;
				}
			}
		}

		for (int i = 0; i < (int) cur->children.size(); i++)
			this->getEtaNode1(cur->children[i], found);
	}
}

std::vector<char*> AST::freeVariables(InternalNode* node) {
	if (node->type == VARIABLE_ID) {
		char *var = ((Leaf*) ((InternalNode*) node)->children[0])->token->value->value.string;
		std::vector<char*> freeVars;
		freeVars.push_back(var);
		return freeVars;
	} else if (node->type == APPLICATION) {
		std::vector<char*> fv1 = this->freeVariables((InternalNode*) (node->children[1]));
		std::vector<char*> fv2 = this->freeVariables((InternalNode*) (node->children[2]));
		fv1.insert(fv1.end(), fv2.begin(), fv2.end());
		return fv1;
	} else if (node->type == ABSTRACTION) {
		char *bindingVar = ((Leaf*) node->children[1])->token->value->value.string;
		std::vector<char*> freeVars = this->freeVariables((InternalNode*) (node->children[2]));
		for (int i = 0; i < (int) freeVars.size(); i++) {
			if (strcmp(freeVars[i], bindingVar) == 0) {
				freeVars.erase(freeVars.begin() + i);
				i--;
			}
		}
		return freeVars;
	} else {
		cout << "INVALID NODE_TYPE" << endl;
		exit(-1);
	}
}

void AST::alpha_convert(InternalNode* abstraction) {
	assert(abstraction->type == ABSTRACTION);

	char *toReplace = ((Leaf*) abstraction->children[1])->token->value->value.string;
	char *save = (char*) malloc(strlen(toReplace));
	strcpy(save, toReplace);
	free(toReplace);

	char *replacement = this->varPool->generateVariable();

	((Leaf*) abstraction->children[1])->token->value->value.string = replacement;

	this->alpha_convert1(abstraction->children[2], save, replacement);
}

void AST::alpha_convert1(Node* node, char* varName, char* replacement) {
	if (dynamic_cast<InternalNode*>(node)) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == VARIABLE_ID) {
			char *toReplace = ((Leaf*) cur->children[0])->token->value->value.string;
			if (strcmp(toReplace, varName) == 0) {
				free(toReplace);
				char *newVar = (char*) malloc(strlen(replacement));
				strcpy(newVar, replacement);
				((Leaf*) cur->children[0])->token->value->value.string = newVar;
			}
		} else if (cur->type == ABSTRACTION) {
			char *bindedVar = ((Leaf*) cur->children[1])->token->value->value.string;
			assert(strcmp(bindedVar, varName));
			this->alpha_convert1(cur->children[2], varName, replacement);
		} else if (cur->type == APPLICATION) {
			this->alpha_convert1(cur->children[1], varName, replacement);
			this->alpha_convert1(cur->children[2], varName, replacement);
		}
	}
}

void AST::replace(InternalNode* toRemove, InternalNode* toInsert) {
	bool replaced = false;
	if (toRemove == (InternalNode*) this->root) {
		delete this->root;
		this->root = (Node*) toInsert;
	} else {
		this->replace1(this->root, toRemove, toInsert, &replaced);
	}
}

void AST::replace1(Node *node, InternalNode* toRemove, InternalNode* toInsert, bool* replaced) {
	if (dynamic_cast<InternalNode*>(node)) {
		InternalNode *cur = (InternalNode*) node;
		for (int i = 0; i < (int)cur->children.size(); i++) {
			if (toRemove == (InternalNode*) cur->children[i]) {
				delete cur->children[i];
				cur->children[i] = toInsert;
				*replaced = true;
				return;
			}
		}
		for (int i = 0; i < (int)cur->children.size(); i++) {
			if (*replaced)
				break;
			this->replace1(cur->children[i], toRemove, toInsert, replaced);
		}

	}
}

vector<InternalNode*> AST::getAllNumberExpressions() {
	vector<InternalNode*> numberExpressions;
	this->getAllNumberExpressions1(this->root, &numberExpressions);
	return numberExpressions;
}

void AST::getAllNumberExpressions1(Node* node, vector<InternalNode*> *vector) {
	if (dynamic_cast<InternalNode*>(node)) {
		InternalNode *cur = (InternalNode*) node;
		if (cur->type == NUMBER_EXP)
			vector->push_back(cur);
		for (int i = 0; i < (int)cur->children.size(); i++)
			this->getAllNumberExpressions1(cur->children[i], vector);
	}
}

