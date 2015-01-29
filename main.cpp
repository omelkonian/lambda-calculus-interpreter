/*
 * main.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <curses.h>
#include <term.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "defines.h"
#include "parser/Parser.h"
#include "tests/Tester.h"

using namespace std;

int main() {

	bool runTest = true;

	if (runTest) {
		Tester *tester = new Tester();
//		tester->testScannerErrorsRT();
//		tester->testScannerRT();
//		tester->testParserRT();
		tester->testParser();
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

