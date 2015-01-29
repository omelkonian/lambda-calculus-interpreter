/*
 * Leaf.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ABSTRACT_SYNTAX_TREE_LEAF_H_
#define ABSTRACT_SYNTAX_TREE_LEAF_H_

#include "Node.h"
#include "../token/Token.h"

/*
 * AST Nodes with no children. Will always hold a token.
 */
class Leaf : public Node {
public:
	Token *token;
	Leaf(Token *token);
	virtual ~Leaf();

	void print();
};

#endif /* ABSTRACT_SYNTAX_TREE_LEAF_H_ */
