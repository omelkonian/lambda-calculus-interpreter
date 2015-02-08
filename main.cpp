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

using namespace std;

int main() {
	// (\x. ((\z. ((k l) m)) x))
	// (((\x. ((x x) y)) (\x. ((x x) y))) y)
	// ((\x. (\y. ((z x) y))) (w y))
	// ((\x. x) ((\y. (y y)) r))
	// (\x. ((\y. y) k))
	// ((\x. (\y. x)) (\z. z))
	// ((\x. x) (\y. y))
	// ((\x. x) ((\y. y) (\z. z)))
	bool runTest = true;

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
		tester->testChurchNumerals();

//		tester->globalTest();
		return 0;
	}

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

		AliasManager *aliasManager = new AliasManager();
		aliasManager->consult("files/prelude.alias");

		string command2(command);
	//	string command("((false x1) x2)");
	//	string command("(false false)");

		command2 = aliasManager->translate(command2);

		char *toExecute = (char*) malloc(strlen(command2.c_str()) + 1);
		strcpy(toExecute, command2.c_str());
		cout << "toExecute: " << toExecute << endl;

		Parser *parser = new Parser(toExecute);
		if (parser->parse()) {
			parser->postProcess();

			parser->printSyntaxTree();

			Evaluator *evaluator = new Evaluator(parser->syntaxTree);
			toExecute = evaluator->evaluate();
			cout << "FINAL: " << toExecute << endl;
		}
		else
			cout << "ERROR: Syntax is wrong" << endl;
		free(command);
	}
}

