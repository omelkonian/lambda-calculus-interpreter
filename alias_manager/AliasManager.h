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
	map<string, string> operatorMap;
 	vector<string> files;
public:
	AliasManager();
	virtual ~AliasManager();

	map<string, string> getOperatorMap();

	void consult(const char *file);

	string deAlias(string name);
	string deAliasOp(string name);
	void addAlias(string translateTo, string toTranslate);
	void addOperator(string translateTo, string toTranslate);

	string translate(string command);
	string deTranslate(string command);

	void printAliases();
	void printOperators();

private:
	string translate1(string command);
	string replace(string command, vector<replaceTuple> replacements);
	vector<Variable*> getVariables(const char *command);
	bool changesMade;
};

#endif /* ALIAS_MANAGER_ALIASMANAGER_H_ */
