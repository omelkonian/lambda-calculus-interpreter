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
#include "../alias_manager/AliasManager.h"

class ChurchNumerator {
	AliasManager *aliasManager;
public:
	AST *syntaxTree;
	ChurchNumerator(AST *syntaxTree, AliasManager *aliasManager);
	virtual ~ChurchNumerator();

	void enchurch();
	void dechurch();

	void printTree();

	static InternalNode *termToNode(string term);
private:
	InternalNode *constructChurchNumeral(int number);

};

#endif /* CHURCH_NUMERALS_CHURCHNUMERATOR_H_ */
