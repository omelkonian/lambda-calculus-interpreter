/*
 * OperatorManager.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: Orestis Melkonian
 */

#include "OperatorManager.h"
#include "../alias_manager/Consultor.h"
#include "../scanner/Scanner.h"
#include <algorithm>
#include <assert.h>

OperatorManager::OperatorManager(AliasManager *aliasManager) {
	this->aliasManager = aliasManager;
}

OperatorManager::~OperatorManager() {
}

string OperatorManager::translate(string term) {
	map<string, string> operatorMap = this->aliasManager->getOperatorMap();
	for (map<string, string>::iterator it = operatorMap.begin(); it != operatorMap.end(); ++it) {
		string oper = it->first;
		int index = term.find(oper);
		int endOfOper = index + (int) oper.size();

		if (term[index + oper.size()] != 32) index = (int)term.npos; // SPACE AFTERWARDS
		if (index != (int) term.npos) {
			bool parenthesisBefore = false;
			string argument;
			assert(index > 0);
			index--;
			while (term[index] == 32)
				index--;

			if (term[index] == '(')
				parenthesisBefore = true;

			if (Scanner::isValidVarSymbol(term[index]) || Scanner::isValidDigit(term[index]) || term[index] == ')') {
				argument += term[index];
				int parCount = 1;
				if (term[index] == ')') {
					while (index >= 0) {
						index--;
						argument += term[index];
						if (term[index] == '(')
							if (--parCount == 0) {
								index--;
								break;
							}
						if (term[index] == ')')
							parCount++;
					}
				} else {
					index--;
					while (index >= 0 && term[index] != 32 && term[index] != '(')
						argument += term[index--];
				}
			}
			if (parenthesisBefore)
				term.replace(index + 1, oper.size(), it->second);
			else {
				string rev(argument);
				reverse(rev.begin(), rev.end());

				string replacement("(");
				replacement += it->second;
				replacement += 32;
				replacement += rev;
				replacement += ") ";

				term.replace(index + 1, endOfOper - index - 1, replacement);
			}
			it--;
		}
	}
	return term;
}
