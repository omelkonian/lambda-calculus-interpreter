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

//#include <curses.h>
//#include <term.h>

#include <readline/readline.h>
#include <readline/history.h>

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

using namespace std;

bool EAGER_EVALUATION;
bool TRACE;
bool LIST_RESULT;

int main() {
	EAGER_EVALUATION = false;
	TRACE = true;

	//((listEqual (\t. ((t true) true))) ((\y. (\t. ((t true) y))) true))

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
		tester->testListChecker();

//		tester->globalTest();


		cout << "------------ALL TESTS PASSED----------" << endl;

		return 0;
	}

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("files/prelude.alias");

	OperatorManager *operatorManager = new OperatorManager(aliasManager);
	ListManager *listManager = new ListManager();

	SystemCommandManager *systemCommandManager = new SystemCommandManager(aliasManager);

	while (true) {
		LIST_RESULT = false;

		char * command = readline("> ");
		if (!command)
			break;
		if (strlen(command) == 0)
			continue;
		if (*command)
			add_history(command);
		if (strcmp(command, "quit") == 0) {
			free(command);
			break;
		}

		string command2(command);

		if (command2[0] == ':')
			systemCommandManager->execute(command2);
		else {
//			cout << "BEF_LIST: " << command2 << endl;
			command2 = listManager->translate(command2);
//			cout << "AFT_LIST: " << command2 << endl;

			cout << "BEF_OPER: " << command2 << endl;
			command2 = operatorManager->translate(command2);
			cout << "AFT_OPER: " << command2 << endl;

//			cout << "BEF_ALIAS: " << command2 << endl;
			command2 = aliasManager->translate(command2);
//			cout << "AFT_ALIAS: " << command2 << endl;

			cerr << "\33[0;4m" << "> " << command2 << "\33[0m";

			char *toExecute = (char*) malloc(strlen(command2.c_str()) + 1);
			strcpy(toExecute, command2.c_str());

			Parser *parser = new Parser(toExecute);
			if (parser->parse()) {
				parser->postProcess();

				ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
				numerator->enchurch();
				cout << "AFT_ENCH: " << parser->syntaxTree->toCommand() << endl;

				Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
				evaluator->evaluate();

				numerator->syntaxTree = parser->syntaxTree;
				numerator->dechurch();

				string final;
				if (LIST_RESULT) {
					cout << "PRINTING LIST" << endl;
					char *com = parser->syntaxTree->toCommand();
					string comStr(com);
					comStr = aliasManager->deTranslate(comStr);
					final = listManager->deTranslate(comStr);
					free(com);
				}
				else {
					char *com = parser->syntaxTree->toCommand();
					string comStr(com);
					final = comStr;
				}


				final = aliasManager->deTranslate(final);
				cerr << "\33[0;31m" << "=>" << "\33[0m";
				cout << final << endl;
				free(toExecute);
			} else
				cout << "ERROR: Syntax is wrong" << endl;
		}
		free(command);
	}
}

