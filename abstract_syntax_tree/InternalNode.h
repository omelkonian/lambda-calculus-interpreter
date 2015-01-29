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
	TERM,
	APPLICATION,
	ABSTRACTION
};

/**
 * Internal node of an AST. Is a term, application or abstraction.
 */
class InternalNode: public Node {
public:
	NodeType type;
	std::vector<Node*> children;

	InternalNode(NodeType type);
	virtual ~InternalNode();

	void addChild(Node *node);

	void print();

private:
	const char* getTypeAsString();
};

#endif /* ABSTRACT_SYNTAX_TREE_INTERNALNODE_H_ */
