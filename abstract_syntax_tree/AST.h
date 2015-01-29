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

	void print();
};

#endif /* ABSTRACT_SYNTAX_TREE_AST_H_ */
