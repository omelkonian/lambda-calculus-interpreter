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
#include <string>
#include "Variable.h"

using namespace std;

typedef pair<string, Range*> replaceTuple;

class AliasManager {
	map<string, string> aliasMap;
	vector<string> files;
public:
	AliasManager();
	virtual ~AliasManager();

	void consult(const char *file);
	string deAlias(string name);
	void addAlias(string translateTo, string toTranslate);
	string translate(string command);

	void printAliases();

private:
	string translate1(string command);
	string replace(string command, vector<replaceTuple> replacements);
	vector<Variable*> getVariables(const char *command);
	bool changesMade;
};

#endif /* ALIAS_MANAGER_ALIASMANAGER_H_ */
