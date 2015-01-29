/*
 * Tester.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#include "Tester.h"
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include <malloc.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "../parser/Parser.h"
#include "../scanner/Scanner.h"
#include "../abstract_syntax_tree/AST.h"
#include "../abstract_syntax_tree/InternalNode.h"
#include "../abstract_syntax_tree/Leaf.h"

using namespace std;

Tester::Tester() {}

Tester::~Tester() {}

void Tester::testScannerErrorsRT() {
	cout << "___________Testing Scanner Error Checking___________" << endl;

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

		Scanner *scanner = new Scanner(command);
		while (Token *token = scanner->nextToken()) {}

		free(command);
	}
}

void Tester::testScannerRT() {
	cout << "___________Testing Scanner___________" << endl;

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

		Scanner *scanner = new Scanner(command);
		while (Token *token = scanner->nextToken())
			token->print();



		free(command);
	}
}

void Tester::testParserRT() {
	cout << "___________Testing Parser___________" << endl;

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

void Tester::testParser() {
	cout << "___________Testing Parser___________" << endl;

	const int commands = 11;
	const int maxCommandLen = 100;

	char **command = (char**) malloc(commands * sizeof(char*));
	for (int i = 0; i < commands; i++)
		command[i] = (char*) malloc(maxCommandLen);

	strcpy(command[0], "(\\ x. (x x) )");
	strcpy(command[1], "((\\ x. (x x) ) (\\ y. (y y) ))");
	strcpy(command[2], "(\\ variable. ((\\ x. (215 + 256) ) (\\ y. (y z) )))");
	strcpy(command[3], "(x + 125)");
	strcpy(command[4], "(\\ x. x.x)");
	strcpy(command[5], "((\\ x. (\\ y. x)) (\\ z. z))");
	strcpy(command[6], "(\\ _ORESTIS_MELKONIAN. LAMBDA_CALCULUS)");
	strcpy(command[7], "( \\ x. ( ( ( a b ) ( c d ) ) ( ( e f ) ( g h ) ) ) )");
	strcpy(command[8], "(\\ x. (((1251 + 125) - 125) - 125) )");
	strcpy(command[9], "(\\ x. x x x)");
	strcpy(command[10], "(\\ x. ((-125) (125 - 5)))");

	bool results[commands];

	Parser *parsers[commands];

	for (int i = 0; i < commands; i++) {
		parsers[i] = new Parser(command[i]);
		results[i] = parsers[i]->parse();
	}

	bool actualResults[commands] = {true, true, true, false, false, true, true, true, true, false, true};

	for (int i = 0; i < commands; i++) {
		assert(results[i] == actualResults[i]);
		cout << "PASS - Command " << i << " - " << command[i] << " is syntactically " << (actualResults[i] ? "correct" : "wrong") << endl;
		delete parsers[i];
	}

	free(command);
}

void Tester::testAST() {
	InternalNode *in = new InternalNode(TERM);
		in->addChild(new Leaf(new Token(LEFT_PAR, 0, new TokenValue(NONE, NULL))));
		InternalNode *in2 = new InternalNode(APPLICATION);
			in2->addChild(new Leaf(new Token(LEFT_PAR, 0, new TokenValue(NONE, NULL))));
			InternalNode *in3 = new InternalNode(TERM);
				char var1[2] = "x";
				in3->addChild(new Leaf(new Token(VARIABLE, 0, new TokenValue(NONE, var1))));
			InternalNode *in4 = new InternalNode(TERM);
				char var2[2] = "y";
				in4->addChild(new Leaf(new Token(VARIABLE, 0, new TokenValue(STRING, var2))));
			in2->addChild(in3);
			in2->addChild(in4);
			in2->addChild(new Leaf(new Token(RIGHT_PAR, 0, new TokenValue(NONE, NULL))));
		in->addChild(in2);
		in->addChild(new Leaf(new Token(RIGHT_PAR, 0, new TokenValue(NONE, NULL))));

	AST *ast = new AST(in);
	ast->print();


}
