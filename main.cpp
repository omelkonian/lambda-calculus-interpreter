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

using namespace std;

bool EAGER_EVALUATION;
bool TRACE;

int main() {
	EAGER_EVALUATION = true;
	TRACE = true;

	// (\x. ((\z. ((k l) m)) x))
	// (((\x. ((x x) y)) (\x. ((x x) y))) y)
	// ((\x. (\y. ((z x) y))) (w y))
	// ((\x. x) ((\y. (y y)) r))
	// (\x. ((\y. y) k))
	// ((\x. (\y. x)) (\z. z))
	// ((\x. x) (\y. y))
	// ((\x. x) ((\y. y) (\z. z)))
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
		tester->testNumericOperations();

//		tester->globalTest();
		return 0;
	}

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("files/prelude.alias");

	SystemCommandManager *systemCommandManager = new SystemCommandManager(aliasManager);

	while (true) {
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

			command2 = aliasManager->translate(command2);

			char *toExecute = (char*) malloc(strlen(command2.c_str()) + 1);
			strcpy(toExecute, command2.c_str());

			Parser *parser = new Parser(toExecute);
			if (parser->parse()) {
				parser->postProcess();

				ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
				numerator->enchurch();

				char *command = parser->syntaxTree->toCommand();
				delete parser->syntaxTree;
				delete parser;
				string commandStr(command);
				string newCommandStr = aliasManager->translate(commandStr);
				free(command);
				char *newCommand = (char*) newCommandStr.c_str();

				parser = new Parser(newCommand);
				parser->parse();
				parser->postProcess();

				Evaluator *evaluator = new Evaluator(parser->syntaxTree);
				evaluator->evaluate();

				numerator->syntaxTree = parser->syntaxTree;
				numerator->dechurch();

				toExecute = parser->syntaxTree->toCommand();

				cout << "=> " << toExecute << endl;
				free(toExecute);
			} else
				cout << "ERROR: Syntax is wrong" << endl;
		}
		free(command);
	}
}

