/*
 * ListManager.h
 *
 *  Created on: 17 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#ifndef LIST_LISTMANAGER_H_
#define LIST_LISTMANAGER_H_

#include "../alias_manager/AliasManager.h"

using namespace std;

class ListManager {
	AliasManager *aliasManager;
public:
	ListManager(AliasManager *aliasManager);
	virtual ~ListManager();

	string translate(string term);
};

#endif /* LIST_LISTMANAGER_H_ */
