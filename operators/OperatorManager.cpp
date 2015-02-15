/*
 * OperatorManager.cpp
 *
 *  Created on: Feb 15, 2015
 *      Author: Orestis Melkonian
 */

#include "OperatorManager.h"
#include "../alias_manager/Consultor.h"

OperatorManager::OperatorManager(AliasManager *aliasManager) {
	this->aliasManager = aliasManager;
}

OperatorManager::~OperatorManager() {
	// TODO Auto-generated destructor stub
}

string OperatorManager::translate(string term) {
	cout << "BEFORE: " << term << endl;
	map<string, string> operatorMap = this->aliasManager->getOperatorMap();
	for (map<string, string>::iterator it = operatorMap.begin(); it != operatorMap.end(); ++it) {
		string oper = it->first;
		// Replace all occurences of <oper> in term.
		term = Consultor::ReplaceString(term, oper, it->second);
	}
	cout << "AFTER: " << term << endl;
	return term;
}
