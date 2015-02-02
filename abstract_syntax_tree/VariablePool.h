/*
 * VariablePool.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ABSTRACT_SYNTAX_TREE_VARIABLEPOOL_H_
#define ABSTRACT_SYNTAX_TREE_VARIABLEPOOL_H_

#include <queue>

/* Variable name generator.
 * Names with format _var<Integer> are reserved and cannot be used by the user.
 */
class VariablePool {
	int counter;
public:
	VariablePool();
	virtual ~VariablePool();

	char* generateVariable();
};

#endif /* ABSTRACT_SYNTAX_TREE_VARIABLEPOOL_H_ */
