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
	else if (command[1] == '[') {
		string alias(command.substr(2, command.size() - 3));
		cout << "alias: " << alias << endl;
		cerr << "\33[0;1;36m" << alias << "\33[0m";
		cout << " -> " << this->aliasManager->deAlias(alias) << endl;
	}
	else if (command.compare(":revealAliases") == 0) {
		this->aliasManager->printAliases();
	}
	else if (command.compare(":revealOperators") == 0) {
		this->aliasManager->printOperators();
	}
	else if (command.compare(":printLimits") == 0) {
		cerr << "\33[0;1;31m" << "MAX_COMMAND_LENGTH: " << "\33[0m";
		cout << MAX_COMMAND_LENGTH << endl;
		cerr << "\33[0;1;31m" << "MAX_VARIABLE_LENGTH: " << "\33[0m";
		cout << MAX_VARIABLE_LENGTH << endl;
		cerr << "\33[0;1;31m" << "MAX_NUMBER_DIGITS: " << "\33[0m";
		cout << MAX_NUMBER_DIGITS << endl;
		cerr << "\33[0;1;31m" << "MAX_NUMBER: " << "\33[0m";
		cout << MAX_NUMBER << endl;
	}
	else if (command.compare(":help") == 0) {
		cout << "\33[0;1;33m" << ":quit" << "\33[0m" << " - Terminates the interpreter" << endl;
		cout << "\33[0;1;33m" << ":[aliasName]" << "\33[0m" << " - Prints term bound to given alias" << endl;
		cout << "\33[0;1;33m" << ":revealAliases" << "\33[0m" << " - Prints all bound variables" << endl;
		cout << "\33[0;1;33m" << ":revealOperators" << "\33[0m" << " - Prints all defined operators" << endl;
		cout << "\33[0;1;33m" << ":printLimits" << "\33[0m" << " - Prints several size limitations" << endl;
		cout << "\33[0;1;33m" << ":trace ON/OFF" << "\33[0m" << " - Prints intermediate reductions" << endl;
		cout << "  Trace Commands: step, abort, run" << endl;
		cout << "\33[0;1;33m" << ":eager ON/OFF" << "\33[0m" << " - ON -> eager evaluation, OFF -> lazy evaluation" << endl;
		cout << "\33[0;1;33m" << ":consult <filename>" << "\33[0m" << "  - Adds all aliases specified at <filename>" << endl;
		cout << "\33[0;1;33m" << ":let <variable> <term>" << "\33[0m" << " - Binds <term> to <variable>" << endl;
		cout << "\33[0;1;33m" << ":debug ON/OFF" << "\33[0m" << " - Prints messages while executing for debugging" << endl;
		cout << "\33[0;1;33m" << ":time ON/OFF" << "\33[0m" << " - Prints the execution time of each command" << endl;
		cout << "\33[0;1;33m" << ":help" << "\33[0m" << " - Display all possible commands" << endl;
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
			TRACE = (!EAGER_EVALUATION) ? true:false;
		else if (flag.compare("OF") == 0)
			TRACE = false;
	}
	else if (command.substr(1, 5).compare("debug") == 0) {
			string flag;
			flag.push_back(command[7]);
			flag.push_back(command[8]);

			if (flag.compare("ON") == 0)
				DEBUG = true;
			else if (flag.compare("OF") == 0)
				DEBUG = false;
	}
	else if (command.substr(1, 5).compare("eager") == 0) {
		string flag;
		flag.push_back(command[7]);
		flag.push_back(command[8]);

		if (flag.compare("ON") == 0) {
			EAGER_EVALUATION = true;
			TRACE = false;
		}
		else if (flag.compare("OF") == 0)
			EAGER_EVALUATION = false;
	}
	else if (command.substr(1, 4).compare("time") == 0) {
			string flag;
			flag.push_back(command[6]);
			flag.push_back(command[7]);

			if (flag.compare("ON") == 0)
				TIME = true;
			else if (flag.compare("OF") == 0)
				TIME = false;
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
