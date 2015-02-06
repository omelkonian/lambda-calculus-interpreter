/*
 * Consultor.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ALIAS_MANAGER_CONSULTOR_H_
#define ALIAS_MANAGER_CONSULTOR_H_

#include "AliasManager.h"

using namespace std;

class Consultor {
	char *filename;
public:
	Consultor(char *filename);
	virtual ~Consultor();

	void getStatements(AliasManager *aliasManager);

private:
	bool checkTerm(char *term);
};

#endif /* ALIAS_MANAGER_CONSULTOR_H_ */
