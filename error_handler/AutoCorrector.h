/*
 * AutoCorrector.h
 *
 *  Created on: Feb 1, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ERROR_HANDLER_AUTOCORRECTOR_H_
#define ERROR_HANDLER_AUTOCORRECTOR_H_

#include <string>
using namespace std;

class AutoCorrector {
	char *command;
public:
	AutoCorrector(char *command);
	virtual ~AutoCorrector();

	char* autoCorrect();

	static bool parBalanced(string term);
	static string removeUnnecessaryParentheses(string term);
	static int getClosingPar(string term, int openingPar);
private:
	void insertSymbolAt(int position, char symbol);
	char getFirstChar();
	int getFirstCharPos();
	char getLastChar();
	int getLastCharPos();
	int getLeftParNo();
	int getRightParNo();
};

#endif /* ERROR_HANDLER_AUTOCORRECTOR_H_ */
