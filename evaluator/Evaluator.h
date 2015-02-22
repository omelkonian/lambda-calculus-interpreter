/*
 * Evaluator.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#ifndef EVALUATOR_EVALUATOR_H_
#define EVALUATOR_EVALUATOR_H_

#include "../abstract_syntax_tree/AST.h"
#include "../alias_manager/AliasManager.h"
#include <string>

class Evaluator {
	AliasManager *aliasManager;
public:
	AST *syntaxTree;

	Evaluator(AST *syntaxTree, AliasManager *aliasManager);
	virtual ~Evaluator();

	char* evaluate();
};

#endif /* EVALUATOR_EVALUATOR_H_ */
