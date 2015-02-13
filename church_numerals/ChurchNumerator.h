/*
 * ChurchNumerator.h
 *
 *  Created on: 7 ��� 2015
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

private:
	InternalNode *constructChurchNumeral(int number);
	InternalNode *termToNode(string term);

};

#endif /* CHURCH_NUMERALS_CHURCHNUMERATOR_H_ */
