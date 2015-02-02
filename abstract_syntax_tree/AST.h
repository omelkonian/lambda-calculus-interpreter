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

	// Checks if there is some beta-reduction in the tree.
	bool bReductionExists();

	// Takes an application that has a b-redex and executes it.
	InternalNode* substitute(InternalNode *node);

	// Returns the free variables of a term.
	std::vector<char*> freeVariables(InternalNode *node);

	// Renames the binded variable of an abstraction to a newly generated variable name.
	void alpha_convert(InternalNode *abstraction);

	InternalNode* getFirstApplication();
	InternalNode* getFirstApplicationParent();

	Node* getRoot();
	void setRoot(Node *root);

	void print();

private:
	InternalNode *parent;

	void refine1(Node *node);
	void doCalculations1(Node *node);
	void simplify1(Node *node);
	void toCommand1(Node *node, int *writePos, char *command);
	void bReductionExists1(Node *node, bool *found);
	void substitute1(InternalNode *node, InternalNode *toInsert, char *varToSub);
	void getFirstApplication1(Node *node, InternalNode **found);
	void getFirstApplicationParent1(Node *node);
	void alpha_convert1(Node *node, char *varName, char *replacement);
};

#endif /* ABSTRACT_SYNTAX_TREE_AST_H_ */
