/*
 * InternalNode.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ABSTRACT_SYNTAX_TREE_INTERNALNODE_H_
#define ABSTRACT_SYNTAX_TREE_INTERNALNODE_H_

#include "Node.h"
#include <vector>

enum NodeType {
	TERM, // Used for parser, specified later.
	APPLICATION,
	ABSTRACTION,
	VARIABLE_ID,
	NUMBER_EXP
};

/**
 * Internal node of an AST. Is a term, application or abstraction.
 */
class InternalNode: public Node {
public:
	NodeType type;
	std::vector<Node*> children;

	InternalNode(NodeType type);
	InternalNode* getNewByCopy();
	virtual ~InternalNode();

	void addChild(Node *node);

	// Returns a number, after doing all required numeric operations recursively.
	int doCalculations();

	void print(int indent);

	const char* getTypeAsString();

private:
	void copy(Node *node, InternalNode **toInsert);
};

#endif /* ABSTRACT_SYNTAX_TREE_INTERNALNODE_H_ */
