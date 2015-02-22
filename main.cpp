/*
 * main.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "linenoise/linenoise.h"

#include "error_handler/error_handling.h"
#include "defines.h"
#include "parser/Parser.h"
#include "tests/Tester.h"
#include "evaluator/Evaluator.h"
#include "error_handler/AutoCorrector.h"
#include "alias_manager/AliasManager.h"
#include "church_numerals/ChurchNumerator.h"
#include "system_commands/SystemCommandManager.h"
#include "operators/OperatorManager.h"
#include "list/ListManager.h"
#include "translator/Translator.h"

using namespace std;

bool EAGER_EVALUATION, TRACE, NON_STOP, DEBUG, ERROR_FOUND, TIME, PURE_TERM;

int main() {
	EAGER_EVALUATION = false;
	TRACE = false;
	NON_STOP = false;
	DEBUG = false;
	TIME = true;
	PURE_TERM = false;

	clock_t begin_time;

	bool runTest = false;

	if (runTest) {
		Tester *tester = new Tester();
//		tester->testScannerErrorsRT();
//		tester->testScannerRT();
//		tester->testParserRT();
//		tester->testParser();
//		tester->testAST();
//		tester->testParserErrors();
//		tester->testAutocorrectorRT();
//		tester->testEvaluatorRT();
//		tester->testEvaluator();
//		tester->testVariablePool();
//		tester->testStringManipulation();
//		tester->testAliasing();
//		tester->testEnchurch();
//		tester->testDechurch();
//		tester->testNumericOperations();
//		tester->testListChecker();
//		tester->testTermConstruction();
//		tester->testPrintList();

//		tester->globalTest();

		delete tester;
		cout << "------------ALL TESTS PASSED----------" << endl;

		return 0;
		exit(0);
	}

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("../files/prelude.alias");

	Translator *translator = new Translator(aliasManager);

	SystemCommandManager *systemCommandManager = new SystemCommandManager(aliasManager);

	char *command;
	while((command = linenoise("> ")) != NULL) {
		if (!command)
			break;
		if (strlen(command) == 0)
			continue;
		if (*command) {
			linenoiseHistoryAdd(command); /* Add to the history. */
			linenoiseHistorySave("history.txt"); /* Save the history on disk. */
		}
		if (strcmp(command, "quit") == 0) {
			free(command);
			break;
		}

		string comStr = command;

		if (comStr[0] == ':')
			systemCommandManager->execute(comStr);
		else {
			begin_time = clock();


			string copy = comStr;

			comStr = translator->translate(comStr);

			if (PURE_TERM && comStr.compare(copy) != 0)
				cerr << "\33[0;1;31m" << "> " << comStr << "\33[0m" << endl;

			char *toExecute = (char*) malloc(strlen(comStr.c_str()) + 1);
			strcpy(toExecute, comStr.c_str());

			ERROR_FOUND = false;

			if (DEBUG) {
				cout << "=======================================================" << endl;
				cout << "PARSING ->" << toExecute << endl;
			}
			Parser *parser = new Parser(toExecute);
			if (DEBUG) cout << "Parser initialized" << endl;
			if (parser->parse()) {
				if (DEBUG) cout << "Parsed" << endl;
				parser->postProcess();
				if (DEBUG) cout << "Post-processed" << endl;
				free(toExecute);

				char *copy, *enchurched;
				if (DEBUG) {
					cout << "--------------ENCHURCH------------" << endl;
					copy = parser->syntaxTree->toCommand();
				}
				ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
				numerator->enchurch();
				if (DEBUG) {
					enchurched = parser->syntaxTree->toCommand();
					if (strcmp(copy, enchurched) != 0) {
						cerr << "\33[0;1;32m" << copy << "\33[0m" << endl;
						cerr << "\33[0;1;33m" << enchurched << "\33[0m" << endl;
					}
					free(copy);
					free(enchurched);
				}

				Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
				free(evaluator->evaluate());

				if (DEBUG) {
					cout << "--------------DECHURCH------------" << endl;
					copy = evaluator->syntaxTree->toCommand();
				}
				numerator->syntaxTree = parser->syntaxTree;
				numerator->dechurch();
				if (DEBUG) {
					enchurched = evaluator->syntaxTree->toCommand();
					if (strcmp(copy, enchurched) != 0) {
						cerr << "\33[0;1;32m" << copy << "\33[0m" << endl;
						cerr << "\33[0;1;33m" << enchurched << "\33[0m" << endl;
					}
					free(copy);
					free(enchurched);
				}

				char *com = parser->syntaxTree->toCommand();
				string final(com);
				free(com);

				string copy1;
				if (DEBUG) {
					cout << "--------------DeTranslate------------" << endl;
					copy1 = final;
				}
				final = aliasManager->deTranslate(final);
				if (DEBUG) {
					if (final.compare(copy1) != 0) {
						cerr << "\33[0;1;32m" << copy1 << "\33[0m" << endl;
						cerr << "\33[0;1;33m" << final << "\33[0m" << endl;
					}
				}

				// Check if result is a list.
				if (ListManager::isList(final))
					final = ListManager::getPrintForm(final);

				if (!ERROR_FOUND) {
					cerr << "\33[0;1;31m" << "=>" << "\33[0m";
					cout << final;
					if (TIME)
						cerr << "\33[0;1;34m" << " (" << float(clock() - begin_time) / CLOCKS_PER_SEC << " seconds)" << "\33[0m";
					cout << endl;
				}
				delete parser;
				delete evaluator;
			} else
				cout << "\33[0;1;31m" << "ERROR" << "\33[0m" << " Syntax is wrong" << endl;
		}
		free(command);
	}
}

