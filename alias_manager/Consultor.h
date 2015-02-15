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
	const char *filename;
public:
	Consultor(const char *filename);
	virtual ~Consultor();

	void getStatements(AliasManager *aliasManager);

	static string ReplaceString(string subject, const string& search, const string& replace);
private:
	bool checkTerm(char *term);
	bool hasRecursion(string variable, string term);
};

#endif /* ALIAS_MANAGER_CONSULTOR_H_ */
