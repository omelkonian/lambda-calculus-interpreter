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
public:
	AutoCorrector();
	virtual ~AutoCorrector();

	static string removeUnnecessaryParentheses(string term);
	static int getClosingPar(string term, int openingPar);
	static int getLeftParNo(string term);
	static int getRightParNo(string term);
};

#endif /* ERROR_HANDLER_AUTOCORRECTOR_H_ */
