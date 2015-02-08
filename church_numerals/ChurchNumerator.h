/*
 * ChurchNumerator.h
 *
 *  Created on: 7 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#ifndef CHURCH_NUMERALS_CHURCHNUMERATOR_H_
#define CHURCH_NUMERALS_CHURCHNUMERATOR_H_

#include "../abstract_syntax_tree/AST.h"
#include "../abstract_syntax_tree/InternalNode.h"

class ChurchNumerator {
	AST *syntaxTree;
public:
	ChurchNumerator(AST *syntaxTree);
	virtual ~ChurchNumerator();

	void termsToNumbers();

private:
	InternalNode *constructChurchNumeral(int number);

};

#endif /* CHURCH_NUMERALS_CHURCHNUMERATOR_H_ */
