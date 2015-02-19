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
	this->operatorManager = new OperatorManager(this);
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

string AliasManager::deAliasOp(string name) {
	for (map<string, string>::iterator it = this->operatorMap.begin(); it != this->operatorMap.end(); ++it) {
		if (name.compare(it->first) == 0) {
			string found(it->second);
			return found;
		}
	}
	return string("");
}

string AliasManager::translate(string command) {
	command = this->translate1(command);
	while (this->changesMade)
		command = this->translate1(command);
	return command;
}

string AliasManager::translate1(string command) {
	vector<replaceTuple> replacements;
	vector<Variable*> variables = this->getVariables(command.c_str());
	// TODO Add operators manager

	for (int i = 0; i < (int) variables.size(); i++)
		variables[i]->print();

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

string AliasManager::deTranslate(string command) {
	for (map<string, string>::iterator it = this->aliasMap.begin(); it != this->aliasMap.end(); ++it) {
		string alias = it->first;
		string deAlias = it->second;

		if (command.find(deAlias) != command.npos) {
			command = Consultor::ReplaceString(command, deAlias, alias);
			it = this->aliasMap.begin();
		}
	}
	return command;
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

void AliasManager::addOperator(string translateTo, string toTranslate) {
	this->operatorMap[toTranslate] = translateTo;
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
	for (map<string, string>::iterator it = this->aliasMap.begin(); it != this->aliasMap.end(); ++it) {
		cerr << "\33[0;36m" << it->first << "\33[0m";
		cout << " => " << it->second << '\n';
	}
}

map<string, string> AliasManager::getOperatorMap() {
	return this->operatorMap;
}

void AliasManager::printOperators() {
	for (map<string, string>::iterator it = this->operatorMap.begin(); it != this->operatorMap.end(); ++it) {
		cerr << "\33[0;36m" << it->first << "\33[0m";
		cout << " => " << it->second << '\n';
	}
}
