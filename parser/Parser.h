/*
 * Parser.h
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "../scanner/Scanner.h"

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
	Parser(char *command);
	virtual ~Parser();

	bool parse();

private:
	bool terminal(TokenType type);

	bool Term();
	bool Term_1();
	bool Term_2();
	bool Term_3();

	bool Number();
	bool Number_1();
	bool Number_2();

	bool X();
	bool X_1();
	bool X_2();

	bool Y();
	bool Y_1();
	bool Y_2();

	bool Z();
	bool Z_1();
	bool Z_2();
};

#endif /* PARSER_PARSER_H_ */
