/*
 * AliasManager.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: Orestis Melkonian
 */

#include "AliasManager.h"
#include "../utilities/Range.h"
#include "../utilities/Utilities.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../scanner/Scanner.h"
#include "../defines.h"
#include "Consultor.h"

#include <map>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;


void printString(char *string) {
	for (int i = 0; i <= (int)strlen(string); i++)
		cout << i << ": " << string[i] << endl;
}

AliasManager::AliasManager() {
	this->changesMade = false;
}

AliasManager::~AliasManager() {
	// TODO free memory
}

void AliasManager::consult(const char *file) {
	Consultor *consultor = new Consultor(file);
	consultor->getStatements(this);
}

string AliasManager::deAlias(string name) {
	for (map<string, string>::iterator it = this->aliasMap.begin(); it != this->aliasMap.end(); ++it) {
		if (name.compare(it->first) == 0) {
			string found(it->second);
			return found;
		}
	}
	return string("");
}

string AliasManager::translate(string command) {
	string ret(command);
	ret = this->translate1(ret);
	while (this->changesMade)
		ret = this->translate1(ret);
	return ret;
}

string AliasManager::translate1(string command) {
	vector<replaceTuple> replacements;
	vector<Variable*> variables = this->getVariables(command.c_str());

	this->changesMade = false;
	for (int i = 0; i < (int)variables.size(); i++) {
		string name = variables[i]->name;
		string alias = this->deAlias(name);

		if (!alias.empty()) {
			changesMade = true;
			replacements.push_back(pair<string, Range*>(alias, variables[i]->range));
		}
	}

	return this->replace(command, replacements);
}

string AliasManager::replace(string command, vector<replaceTuple> replacements) {
	for (int i = (int)replacements.size() - 1; i >= 0; i--) {
		Range *range = replacements[i].second;
		command.replace(range->min, range->max - range->min + 1, replacements[i].first);
	}
	return command;
}

void AliasManager::addAlias(string translateTo, string toTranslate) {
	this->aliasMap[toTranslate] = translateTo;
}

vector<Variable*> AliasManager::getVariables(const char *command) {
	int i = 0;
	vector<char*> variables;
	vector<Range*> ranges;
	bool variableFound = false;
	char *var = NULL;
	int minRange;
	int maxRange;
	while (i < (int) strlen(command)) {
		if (Scanner::isValidVarSymbol(command[i]) || Scanner::isValidDigit(command[i])) {
			if (!variableFound && Scanner::isValidVarSymbol(command[i])) {
				variableFound = true;
				minRange = i;
				maxRange = i;
				var = (char*) malloc(MAX_COMMAND_LENGTH);
				var[0] = command[i];
				var[1] = '\0';
			} else if (Scanner::isValidVarSymbol(command[i])){
				maxRange++;
				int end = (int) strlen(var);
				var[end] = command[i];
				var[end + 1] = '\0';
			}
		} else if (variableFound) {
			variableFound = false;
			assert(var);
			variables.push_back(var);
			Range *range = new Range(minRange, maxRange);
			ranges.push_back(range);
			var = NULL;
		}
		i++;
	}
	if (variableFound) {
		assert(var);
		variables.push_back(var);
		Range *range = new Range(minRange, maxRange);
		ranges.push_back(range);
	}

	// Construct var vector
	vector<Variable*> ret;
	for (int i = 0; i < (int) variables.size(); i++) {
		string toInsert(variables[i]);
		Variable *variable = new Variable(toInsert, ranges[i]);
		free(variables[i]);
		ret.push_back(variable);
	}

	return ret;
}

void AliasManager::printAliases() {
	for (map<string, string>::iterator it = this->aliasMap.begin(); it != this->aliasMap.end(); ++it)
		cout << it->first << " => " << it->second << '\n';
}
