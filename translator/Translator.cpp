/*
 * Translator.cpp
 *
 *  Created on: 21 Öåâ 2015
 *      Author: Orestis Melkonian
 */

#include "Translator.h"
#include "../defines.h"

Translator::Translator(AliasManager* aliasManager) {
	this->aliasManager = aliasManager;
	this->operatorManager = new OperatorManager(aliasManager);
}

Translator::~Translator() {
}

string Translator::translate(string term) {
	string outerCopy("$$$$$$$$$");

	while (outerCopy.compare(term) != 0) {
		string copy;
		outerCopy = term;
		term = AutoCorrector::removeUnnecessaryParentheses(term);
		if (DEBUG) {
			cout << "--------------LIST---------------" << endl;
			copy = term;
		}
		term = ListManager::translate(term);
		if (DEBUG && copy.compare(term) != 0) {
			cerr << "\33[0;1;32m" << copy << "\33[0m" << endl;
			cerr << "\33[0;1;33m" << term << "\33[0m" << endl << endl;
		}
		term = AutoCorrector::removeUnnecessaryParentheses(term);

		if (DEBUG) {
			cout << "--------------OPER---------------" << endl;
			copy = term;
		}
		term = operatorManager->translate(term);
		if (DEBUG && copy.compare(term) != 0) {
			cerr << "\33[0;1;32m" << copy << "\33[0m" << endl;
			cerr << "\33[0;1;33m" << term << "\33[0m" << endl;
		}
		term = AutoCorrector::removeUnnecessaryParentheses(term);

		if (DEBUG) {
			cout << "--------------ALIAS--------------" << endl;
			copy = term;
		}
		term = aliasManager->translate(term);
		if (DEBUG && copy.compare(term) != 0) {
			cerr << "\33[0;1;32m" << copy << "\33[0m" << endl;
			cerr << "\33[0;1;33m" << term << "\33[0m" << endl;
		}
		term = AutoCorrector::removeUnnecessaryParentheses(term);
	}

	return term;
}
