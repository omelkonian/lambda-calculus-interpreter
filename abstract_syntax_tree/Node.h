/*
 * Node.h
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ABSTRACT_SYNTAX_TREE_NODE_H_
#define ABSTRACT_SYNTAX_TREE_NODE_H_

/*
 * Abstract class.
 */
class Node {
public:
	Node();
	virtual ~Node();

	virtual void print() {};
};

#endif /* ABSTRACT_SYNTAX_TREE_NODE_H_ */
