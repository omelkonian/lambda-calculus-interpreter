/*
 * AliasManager.h
 *
 *  Created on: Feb 4, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ALIAS_MANAGER_ALIASMANAGER_H_
#define ALIAS_MANAGER_ALIASMANAGER_H_

#include <vector>
#include <map>
#include "Variable.h"

using namespace std;

class AliasManager {
	map<char*, const char*> aliasMap;
	vector<char*> files;
public:
	AliasManager();
	virtual ~AliasManager();

	void consult(char *file);
	char* deAlias(char *name);
	void addAlias(char *translateTo, char *toTranslate);
	char* translate(char *command);

	void printAliases();

private:
	char* translate1(char *command);
	char* replace(char *command, map<char*, Range*> replacements, vector<char*> varsToReplace);
	vector<Variable*> getVariables(char *command);
	bool changesMade;
};

#endif /* ALIAS_MANAGER_ALIASMANAGER_H_ */
