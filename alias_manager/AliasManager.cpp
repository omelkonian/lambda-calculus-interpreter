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
	// TODO Auto-generated destructor stub
}

void AliasManager::consult(char *file) {
	Consultor *consultor = new Consultor(file);
	consultor->getStatements(this);
}

char* AliasManager::deAlias(char *name) {
	for (map<char*, const char*>::iterator it = this->aliasMap.begin(); it != this->aliasMap.end(); ++it) {
		if (strcmp(name, it->first) == 0) {
			const char *found = it->second;
			char *ret = (char*) malloc(strlen(found) + 1);
			strcpy(ret, found);
			ret[strlen(found)] = '\0';
			return ret;
		}
	}
	return NULL;
}

char* AliasManager::translate(char* command) {
	char *copy = (char*) malloc(MAX_COMMAND_LENGTH);
	strcpy(copy, command);
	command = this->translate1(command);
	while (strcmp(copy, command)) {
		strcpy(copy, command);
		command = this->translate1(command);
	}
	free(copy);
	return command;
}

char* AliasManager::translate1(char *command) {
	map<char*, Range*> replacements;
	vector<char*> variablesToReplace;
	vector<Variable*> variables = this->getVariables(command);

	this->changesMade = false;
	for (int i = 0; i < (int) variables.size(); i++) {
		char *name = variables[i]->name;
		char *alias = this->deAlias(name);

		if (alias) {
			changesMade = true;
			replacements[alias] = variables[i]->range;
			variablesToReplace.push_back(alias);
		}
	}

	return this->replace(command, replacements, variablesToReplace);
}

char* AliasManager::replace(char* command, map<char*, Range*> replacements, vector<char*> varsToReplace) {
	for (int i = (int) varsToReplace.size() - 1; i >= 0; i--) {
		Range *range = replacements[varsToReplace[i]];
		command = Utilities::remove(command, range);
		command = Utilities::insertAt(command, varsToReplace[i], range->min);
	}
	return command;
}

void AliasManager::addAlias(char* translateTo, char* toTranslate) {
	this->aliasMap[toTranslate] = (const char*) translateTo;
}

vector<Variable*> AliasManager::getVariables(char *command) {
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
			} else {
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
		Variable *variable = new Variable(variables[i], ranges[i]);
		ret.push_back(variable);
	}

	return ret;
}

void AliasManager::printAliases() {
	for (map<char*, const char*>::iterator it = this->aliasMap.begin(); it != this->aliasMap.end(); ++it)
		cout << it->first << " => " << it->second << '\n';
}
