/*
 * AST.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ABSTRACT_SYNTAX_TREE_AST_H_
#define ABSTRACT_SYNTAX_TREE_AST_H_

#include "Node.h"

class AST {
	Node *root;
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

	Node* getRoot();

	void print();

private:
	void refine1(Node *node);
	void doCalculations1(Node *node);
	void simplify1(Node *node);
	void toCommand1(Node *node, int *writePos, char *command);
};

#endif /* ABSTRACT_SYNTAX_TREE_AST_H_ */
