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

using namespace std;

int main() {
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
//		tester->testAutocorrector();
		tester->testEvaluatorRT();
//		tester->testVariablePool();
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

		Parser *parser = new Parser(command);

		cout << "Syntax is " << (parser->parse() ? "correct." : "wrong.") << endl;

		free(command);
	}
}

