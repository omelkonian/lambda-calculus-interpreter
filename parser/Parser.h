/*
 * Parser.h
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "../scanner/Scanner.h"
#include "../abstract_syntax_tree/AST.h"
#include "../abstract_syntax_tree/InternalNode.h"
#include "../abstract_syntax_tree/Leaf.h"

/*
 * Recursive Descent Parser.
 *
 * ----------GRAMMAR (left-factoring required for Recursive Descent Method ) -----------
 *
 *  Term := variable
 * 		  | Number
 *  	  | ( X
 *
 *  Number := number
 * 		    | ( Number Z
 *
 * 	X := Term Y
 * 	   | \ variable . Term
 *
 * 	Y := Term )
 * 	   | )
 *
 * 	Z := )
 * 	   | operator Number )
 *
 */
class Parser {
	Scanner *scanner;
	Token **next;
	int savedTokens;
	int curIndex;

public:
	AST *syntaxTree;

	Parser(char *command);
	virtual ~Parser();

	bool parse();

	void postProcess();

	void printSyntaxTree();

private:
	bool terminal(TokenType type, InternalNode *node);

	bool Term(InternalNode *node);
	bool Term_1(InternalNode *node);
	bool Term_2(InternalNode *node);
	bool Term_3(InternalNode *node);

	bool Number(InternalNode *node);
	bool Number_1(InternalNode *node);
	bool Number_2(InternalNode *node);

	bool X(InternalNode *node);
	bool X_1(InternalNode *node);
	bool X_2(InternalNode *node);

	bool Y(InternalNode *node);
	bool Y_1(InternalNode *node);
	bool Y_2(InternalNode *node);

	bool Z(InternalNode *node);
	bool Z_1(InternalNode *node);
	bool Z_2(InternalNode *node);
};

#endif /* PARSER_PARSER_H_ */
