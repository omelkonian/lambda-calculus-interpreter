/*
 * Evaluator.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Orestis Melkonian
 */

#ifndef EVALUATOR_EVALUATOR_H_
#define EVALUATOR_EVALUATOR_H_

#include "../abstract_syntax_tree/AST.h"

class Evaluator {
	AST *syntaxTree;
public:
	Evaluator(AST *syntaxTree);
	virtual ~Evaluator();

	char* evaluate();
};

#endif /* EVALUATOR_EVALUATOR_H_ */
