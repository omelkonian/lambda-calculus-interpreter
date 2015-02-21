/*
 * Translator.h
 *
 *  Created on: 21 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#ifndef TRANSLATOR_TRANSLATOR_H_
#define TRANSLATOR_TRANSLATOR_H_

#include "../alias_manager/AliasManager.h"
#include "../operators/OperatorManager.h"
#include "../error_handler/AutoCorrector.h"
#include "../list/ListManager.h"
#include <string>

using namespace std;

class Translator {
	AliasManager *aliasManager;
	OperatorManager *operatorManager;
public:
	Translator(AliasManager *aliasManager);
	virtual ~Translator();

	string translate(string term);
};

#endif /* TRANSLATOR_TRANSLATOR_H_ */
