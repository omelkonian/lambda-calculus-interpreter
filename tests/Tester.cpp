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
#include "../defines.h"
#include "../error_handler/AutoCorrector.h"
#include "../evaluator/Evaluator.h"
#include "../abstract_syntax_tree/VariablePool.h"
#include "../utilities/Range.h"
#include "../utilities/Utilities.h"
#include "../alias_manager/AliasManager.h"
#include "../church_numerals/ChurchNumerator.h"
#include "../list/ListManager.h"

using namespace std;

Tester::Tester() {
}

Tester::~Tester() {
}

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
		while (Token *token = scanner->nextToken()) {
		}

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
	// Must comment out exit(-1) in print_error.
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

	bool actualResults[commands] = { true, true, true, false, false, true, true, true, true, false, true };

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
	// Must comment out exit(-1) in print_error.
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
	strcpy(command[16], "((\\x. x) (\\y. *))");
	strcpy(command[17], "((\\x. x) (\\y.))");
	strcpy(command[18], "((\\x. x) (\\y. \\))");
	strcpy(command[19], "((\\x. x) (\\y..))");
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

	char *command = (char*) malloc(MAX_COMMAND_LENGTH);
	char *result = (char*) malloc(MAX_COMMAND_LENGTH);

	strcpy(command, "(1 * (1 + ((100) / (10 % 100))");
	strcpy(result, "(11)");

	Parser *parser = new Parser(command);
	assert(parser->parse());
	parser->postProcess();

	assert(strcmp(result, parser->syntaxTree->toCommand()) == 0);

	delete parser;
	free(result);
	free(command);
}

void Tester::testAutocorrectorRT() {
	cout << "___________Testing AutoCorrector Real-Time___________" << endl;

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

		AutoCorrector *ac = new AutoCorrector(command);
		command = ac->autoCorrect();
		cout << endl;
		cout << "Final: " << command << endl;
	}

}

void Tester::testEvaluatorRT() {
	cout << "___________Testing Evaluator Real-Time___________" << endl;

//	while (true) {
//		char * command = readline("> ");
//		if (!command)
//			break;
//		if (strlen(command) == 0)
//			continue;
//		if (*command)
//			add_history(command);
//		if (strcmp(command, "quit") == 0) {
//			free(command);
//			break;
//		}
//
//		AutoCorrector *ac = new AutoCorrector(command);
//		command = ac->autoCorrect();
//
//		Parser *parser = new Parser(command);
//		parser->parse();
//		parser->postProcess();
//
//		parser->printSyntaxTree();
//
//		Evaluator *eval = new Evaluator(parser->syntaxTree);
//		char *finalCommand = eval->evaluate();
//
//		cout << "FINAL: " << finalCommand << endl;
//
//		free(command);
//	}
}

void Tester::testVariablePool() {
	cout << "___________Testing Variable Pool___________" << endl;

	VariablePool *varPool = new VariablePool();
	cout << varPool->generateVariable() << endl;
	cout << varPool->generateVariable() << endl;
	cout << varPool->generateVariable() << endl;
	cout << varPool->generateVariable() << endl;
	cout << varPool->generateVariable() << endl;
	cout << varPool->generateVariable() << endl;
	cout << varPool->generateVariable() << endl;
}

void Tester::testEvaluator() {
	cout << "___________Testing Evaluator___________" << endl;

	const int commands = 7;
	const int maxCommandLen = 100;

	char **command = (char**) malloc(commands * sizeof(char*));
	for (int i = 0; i < commands; i++)
		command[i] = (char*) malloc(maxCommandLen);

	strcpy(command[0], "((\\x. x) (\\y. (y y)))");
	strcpy(command[1], "((\\w. y) ((\\x. (x x)) (\\x. (x x))))");
	strcpy(command[2], "(\\x. ((\\y. y) k))");
	strcpy(command[3], "((\\x. x) ((\\y. (y y)) r))");
	strcpy(command[4], "((\\x. ((\\y. (y x)) w)) f)");
	strcpy(command[5], "((\\x. x) x)");
	strcpy(command[6], "((\\x. (\\y. ((z x) y))) (w y))");

	char **actualCommand = (char**) malloc(commands * sizeof(char*));
	for (int i = 0; i < commands; i++)
		actualCommand[i] = (char*) malloc(maxCommandLen);

	strcpy(actualCommand[0], "(\\y. (y y))");
	strcpy(actualCommand[1], "y");
	strcpy(actualCommand[2], "(\\x. k)");
	strcpy(actualCommand[3], "(r r)");
	strcpy(actualCommand[4], "(w f)");
	strcpy(actualCommand[5], "x");
	strcpy(actualCommand[6], "(\\_var0. ((z (w y)) _var0))");

	for (int i = 0; i < commands; i++) {
		cout << "> " << command[i] << endl;

//		Parser *parser = new Parser(command[i]);
//		assert(parser->parse());
//		parser->postProcess();
//
//		Evaluator *evaluator = new Evaluator(parser->syntaxTree);
//
//		command[i] = evaluator->evaluate();
//
//		delete parser;
//		delete evaluator;
	}

	for (int i = 0; i < commands; i++) {
		assert(strcmp(command[i], actualCommand[i]) == 0);
		free(command[i]);
		free(actualCommand[i]);
	}
	free(actualCommand);
	free(command);
}

void Tester::testStringManipulation() {
	char *command = (char*) malloc(9);
	strcpy(command, "123456789");
	command = Utilities::remove(command, new Range(1, 5));

	char *toInsert = (char*) malloc(7);
	strcpy(toInsert, "abcdef");
	command = Utilities::insertAt(command, toInsert, 4);

	assert(strcmp(command, "3456abcdef789") == 0);
}

void Tester::testAliasing() {
	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("files/prelude.alias");

	string command("(snd ((pair x1) x2))");
//	string command("((false x1) x2)");
//	string command("(false false)");

	command = aliasManager->translate(command);

	char *toExecute = (char*) malloc(strlen(command.c_str()) + 1);
	strcpy(toExecute, command.c_str());
	cout << "toExecute: " << toExecute << endl;

	Parser *parser = new Parser(toExecute);
	if (parser->parse()) {
		parser->postProcess();

		Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
		toExecute = evaluator->evaluate();
		cout << "FINAL: " << toExecute << endl;
	} else
		cout << "ERROR: Syntax is wrong" << endl;

	assert(strcmp(toExecute, "x2") == 0);
}

void Tester::testEnchurch() {
	string command("(((\\x.(\\y. y)) 1) 2)");

	char *toExecute = (char*) malloc(strlen(command.c_str()) + 1);
	strcpy(toExecute, command.c_str());

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("files/prelude.alias");

	Parser *parser = new Parser(toExecute);
	if (parser->parse()) {
		parser->postProcess();

		ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
		numerator->enchurch();

		cout << parser->syntaxTree->toCommand() << endl;

		Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
		toExecute = evaluator->evaluate();
	} else
		cout << "ERROR: Syntax is wrong" << endl;

	assert(strcmp(toExecute, "(\\f. (\\x. (f (f x))))") == 0);
}

void Tester::testDechurch() {
	string command("(\\f. (\\x. (f (f (f (f (f (f (f (f (f (f x))))))))))))");

	char *toExecute = (char*) malloc(strlen(command.c_str()) + 1);
	strcpy(toExecute, command.c_str());

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("files/prelude.alias");

	Parser *parser = new Parser(toExecute);
	if (parser->parse()) {
		parser->postProcess();

		ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
		numerator->dechurch();
		numerator->printTree();
	} else
		cout << "ERROR: Syntax is wrong" << endl;
}

void Tester::testNumericOperations() {
//	string command("(((5 + (10 - 5)) ^ 2) * (10 * (5 - 4)))");
//	string command("(10 * (10 * 1))");
//	string command("(10 - (10 * 1))");
	string command("(10 + (10 - 1))");
	cout << "> " << command << endl;

	char *toExecute = (char*) malloc(strlen(command.c_str()) + 1);
	strcpy(toExecute, command.c_str());

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("files/prelude.alias");

	Parser *parser = new Parser(toExecute);
	if (parser->parse()) {
		parser->postProcess();

		parser->printSyntaxTree();

		ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
		numerator->enchurch();

		parser->printSyntaxTree();

		Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
		free(evaluator->evaluate());
		numerator->dechurch();

		parser->printSyntaxTree();

		toExecute = parser->syntaxTree->toCommand();
		assert(strcmp(toExecute, "1000") == 0);
	} else
		cout << "ERROR: Syntax is wrong" << endl;
}

void Tester::globalTest() {
//	this->testCalculator();
//	this->testParser();
	this->testEvaluator();
}

void Tester::testListChecker() {
	vector<string> lists;
	lists.push_back("[1, 2, 3, 4, 5]");
	lists.push_back("[1]");
	lists.push_back("[1,2,3,4,5]");
	lists.push_back("[1,  2,  3,  4,  5]");
	lists.push_back("[1, 2, 1235,     12,    sdaflgad, 1,1,1,1,1]");

	for (int i = 0; i < (int) lists.size(); i++) {
		string list = lists[i];
		ListManager *listM = new ListManager();
		list = listM->translate(list);
		assert(Evaluator::isList(list));
	}
}
