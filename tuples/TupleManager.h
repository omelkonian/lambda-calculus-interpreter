/*
 * TupleManager.h
 *
 *  Created on: 17 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#ifndef TUPLES_TUPLEMANAGER_H_
#define TUPLES_TUPLEMANAGER_H_

#include "../alias_manager/AliasManager.h"

using namespace std;

class TupleManager {
	AliasManager *aliasManager;
public:
	TupleManager(AliasManager *aliasManager);
	virtual ~TupleManager();

	string translate(string term);
};

#endif /* TUPLES_TUPLEMANAGER_H_ */
