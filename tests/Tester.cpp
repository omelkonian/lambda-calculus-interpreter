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
#include <string.h>
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
	cout << "___________Testing Scanner Real-Time___________" << endl;

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
	cout << "___________Testing Parser Real-Time___________" << endl;

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

		parser->postProcess();
		parser->printSyntaxTree();

		cout << "Command: " << parser->syntaxTree->toCommand() << endl;

		delete parser;
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
				in3->addChild(new Leaf(new Token(VARIABLE, 0, new TokenValue(STRING, var1))));
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

void Tester::testParserErrors() {
	cout << "___________Testing Parser Error Checking___________" << endl;

	const int commands = 24;
	const int maxCommandLen = 100;

	char **command = (char**) malloc(commands * sizeof(char*));
	for (int i = 0; i < commands; i++)
		command[i] = (char*) malloc(maxCommandLen);

	strcpy(command[0], "((\\x. x) ())");
	strcpy(command[1], "((\\x. x) (.))");
	strcpy(command[2], "((\\x. x) (+))");
	strcpy(command[3], "((\\x. x) (\\()))");
	strcpy(command[4], "((\\x. x) (\\))");
	strcpy(command[5], "((\\x. x) (\\ \\))");
	strcpy(command[6], "((\\x. x) (\\ .))");
	strcpy(command[7], "((\\x. x) (\\ %))");
	strcpy(command[8], "((\\x. x) (\\ 15692))");
	strcpy(command[9], "((\\x. x) (y \\))");
	strcpy(command[10], "((\\x. x) (y *))");
	strcpy(command[11], "((\\x. x) (1 +))");
	strcpy(command[12], "((\\x. x) (1 + \\))");
	strcpy(command[13], "((\\x. x) (1 + .))");
	strcpy(command[14], "((\\x. x) (1 + y))");
	strcpy(command[15], "((\\x. x) (1 + *))");
	strcpy(command[16], "((\\x. x) (\y. *))");
	strcpy(command[17], "((\\x. x) (\y.))");
	strcpy(command[18], "((\\x. x) (\y. \\))");
	strcpy(command[19], "((\\x. x) (\y..))");
	strcpy(command[20], "((\\x. x) (25 \\))");
	strcpy(command[21], "((\\x. x) (25 .))");
	strcpy(command[22], "((\\x. x) (x y)\\)");
	strcpy(command[23], "((\\x. x) (x).)");

	Parser *parsers[commands];

	for (int i = 0; i < commands; i++) {
		cout << "> " << command[i] << endl;
		parsers[i] = new Parser(command[i]);
		assert(!parsers[i]->parse());
	}

	free(command);
}

void Tester::testCalculator() {
	cout << "___________Testing Calculator___________" << endl;

	char *command = (char*) malloc(50);

	strcpy(command, "(1 * (1 + ((100) / (10 % 100))");

	Parser *parser = new Parser(command);
	assert(parser->parse());
	parser->postProcess();


	free(command);
}
