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
	AST *syntaxTree;
	AliasManager *aliasManager;
public:
	Evaluator(AST *syntaxTree, AliasManager *aliasManager);
	virtual ~Evaluator();

	char* evaluate();

//private:
	static bool isList(string term);
};

#endif /* EVALUATOR_EVALUATOR_H_ */
