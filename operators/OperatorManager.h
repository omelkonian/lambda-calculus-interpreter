/*
 * OperatorManager.h
 *
 *  Created on: Feb 15, 2015
 *      Author: Orestis Melkonian
 */

#ifndef OPERATORS_OPERATORMANAGER_H_
#define OPERATORS_OPERATORMANAGER_H_

#include <string>
#include <map>
#include "../alias_manager/AliasManager.h"

using namespace std;

class OperatorManager {
	AliasManager *aliasManager;
public:
	OperatorManager(AliasManager *aliasManager);
	virtual ~OperatorManager();

	string translate(string term);
};

#endif /* OPERATORS_OPERATORMANAGER_H_ */
