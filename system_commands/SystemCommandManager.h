/*
 * SystemCommandManager.h
 *
 *  Created on: Feb 13, 2015
 *      Author: Orestis Melkonian
 */

#ifndef SYSTEM_COMMANDS_SYSTEMCOMMANDMANAGER_H_
#define SYSTEM_COMMANDS_SYSTEMCOMMANDMANAGER_H_

#include "../alias_manager/AliasManager.h"
#include <string>

class SystemCommandManager {
	AliasManager *aliasManager;
public:
	SystemCommandManager(AliasManager *aliasManager);
	virtual ~SystemCommandManager();

	void execute(string command);
};

#endif /* SYSTEM_COMMANDS_SYSTEMCOMMANDMANAGER_H_ */
