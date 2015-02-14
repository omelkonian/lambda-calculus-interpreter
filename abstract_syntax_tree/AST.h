/*
 * AST.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ABSTRACT_SYNTAX_TREE_AST_H_
#define ABSTRACT_SYNTAX_TREE_AST_H_

#include "Node.h"
#include "InternalNode.h"
#include "VariablePool.h"
#include <vector>

using namespace std;

class AST {
	Node *root;
	VariablePool *varPool;
public:
	AST(Node *root);
	virtual ~AST();

	// Differentiates abstractions from applications.
	void refine();

	// Calculates all NUMBER_EXP inside the tree.
	void doCalculations();

	// Cuts off unnecessary nodes(LAMBDA_DOT,...)
	void simplify();

	// Returns the lambda-term this tree represents as string.
	char* toCommand();

	// Checks if there is a beta-reduction/eta-conversion in the tree.
	bool bReductionExists();
	bool etaConversionExists();

	// Takes an application that has a b-redex and executes it.
	InternalNode* substitute(InternalNode *node);

	// Executes an eta-conversion on given node.
	void eta_convert(InternalNode *application);

	// Renames the binded variable of an abstraction to a newly generated variable name.
	void alpha_convert(InternalNode *abstraction);

	// Replaces a subtree with another subtree.
	void replace(InternalNode *toRemove, InternalNode *toInsert);

	// Returns the free variables of a term.
	std::vector<char*> freeVariables(InternalNode *node);


	InternalNode* getFirstApplication();
	InternalNode* getEtaNode();

	Node* getRoot();
	void setRoot(Node *root);

	InternalNode* getParent(Node *child);

	vector<InternalNode*> getAllNumberExpressions();
	vector<pair<InternalNode*, int> > getAllChurchNumerals();

	void print();

private:
	InternalNode *parent;

	void refine1(Node *node);
	void doCalculations1(Node *node);
	void simplify1(Node *node);

	void bReductionExists1(Node *node, bool *found);
	void etaConversionExists1(Node *node, bool *found);

	void substitute1(InternalNode *node, InternalNode *toInsert, char *varToSub);
	void alpha_convert1(Node *node, char *varName, char *replacement);

	void getFirstApplication1(Node *node, InternalNode **found);
	void getEtaNode1(Node *node, InternalNode **found);

	void getParent1(Node *cur, Node *child, InternalNode **parent);
	void toCommand1(Node *node, int *writePos, char *command);

	void replace1(Node *node, InternalNode *toRemove, InternalNode *toInsert, bool *replaced);

	void getAllNumberExpressions1(Node *node, std::vector<InternalNode*> *vector);
	void getAllChurchNumerals1(Node *node, vector<pair<InternalNode*, int> > *vector);

	int isChurchNumeral(InternalNode *node);
};

#endif /* ABSTRACT_SYNTAX_TREE_AST_H_ */
