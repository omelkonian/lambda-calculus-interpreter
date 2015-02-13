/*
 * SystemCommandManager.cpp
 *
 *  Created on: Feb 13, 2015
 *      Author: Orestis Melkonian
 */

#include "SystemCommandManager.h"
#include <assert.h>
#include "../defines.h"

SystemCommandManager::SystemCommandManager(AliasManager* aliasManager) {
	this->aliasManager = aliasManager;
}

SystemCommandManager::~SystemCommandManager() {
}

void SystemCommandManager::execute(string command) {
	assert(command[0] == ':');
	if (command.compare(":quit") == 0) {
		exit(0);
	}
	else if (command.compare(":revealAliases") == 0) {
		this->aliasManager->printAliases();
	}
	else if (command.compare(":help") == 0) {
		cout << ":quit - Terminates the interpreter" << endl;
		cout << ":revealAliases - Prints all bound variables" << endl;
		cout << ":trace ON/OFF - If ON, prints every intermediate reduction" << endl;
		cout << ":eager ON/OFF - ON -> eager evaluation, OFF -> lazy evaluation" << endl;
		cout << ":consult <filename>  - Adds all aliases specified at <filename>" << endl;
		cout << ":let <variable> <term> - Binds <term> to <variable>" << endl;
		cout << ":help - Display all possible commands" << endl;
	}
	else if (command.substr(1, 3).compare("let") == 0) {
		string variable;
		string term;
		int index = 5;
		while (command[index] != 32)
			variable.push_back(command[index++]);
		while (command[index] == 32) index++;
		while (index != (int)command.size())
			term.push_back(command[index++]);

		this->aliasManager->addAlias(term, variable);
	}
	else if (command.substr(1, 5).compare("trace") == 0) {
		string flag;
		flag.push_back(command[7]);
		flag.push_back(command[8]);

		if (flag.compare("ON") == 0)
			TRACE = true;
		else if (flag.compare("OF") == 0)
			TRACE = false;
	}
	else if (command.substr(1, 5).compare("eager") == 0) {
		string flag;
		flag.push_back(command[7]);
		flag.push_back(command[8]);

		if (flag.compare("ON") == 0)
			EAGER_EVALUATION = true;
		else if (flag.compare("OFF") == 0)
			EAGER_EVALUATION = false;
	}
	else if (command.substr(1, 7).compare("consult") == 0) {
		string filename;
		int index = 9;
		while (index != (int)command.size())
			filename.push_back(command[index++]);
		this->aliasManager->consult(filename.c_str());
	}
	else
		cout << "ERROR: Invalid command - Type ':help' for a list of all available commands." << endl;
}
