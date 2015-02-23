///*
// * Tester.cpp
// *
// *  Created on: Jan 28, 2015
// *      Author: Orestis Melkonian
// */
//
#include "Tester.h"
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <malloc.h>

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
#include "../alias_manager/AliasManager.h"
#include "../church_numerals/ChurchNumerator.h"
#include "../list/ListManager.h"
#include "../translator/Translator.h"

using namespace std;

Tester::Tester() {
}

Tester::~Tester() {
}

void Tester::testParserErrors() {
	cout << "___________Testing Parser Error-Checking___________" << endl;

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
		ERROR_FOUND = false;
		cout << "> " << command[i] << endl;
		parsers[i] = new Parser(command[i]);
		assert(!parsers[i]->parse());
	}

	free(command);
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
	strcpy(actualCommand[6], "(z (w y))");

	AliasManager *aliasManager = new AliasManager();

	for (int i = 0; i < commands; i++) {
		ERROR_FOUND = false;
		cout << "> " << command[i] << endl;

		Parser *parser = new Parser(command[i]);
		assert(parser->parse());
		parser->postProcess();

		Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);

		command[i] = evaluator->evaluate();

		cout << "-> " << command[i] << endl << endl;

		delete parser;
		delete evaluator;
	}

	for (int i = 0; i < commands; i++) {
		assert(strcmp(command[i], actualCommand[i]) == 0);
		free(command[i]);
		free(actualCommand[i]);
	}
	free(actualCommand);
	free(command);
}

void Tester::testAliasing() {
	cout << "___________Testing Aliasing___________" << endl;
	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("../files/prelude.alias");

	string command("(snd ((pair x1) x2))");
	cout << "> " << command << endl;

	command = aliasManager->translate(command);

	char *toExecute = (char*) malloc(strlen(command.c_str()) + 1);
	strcpy(toExecute, command.c_str());

	Parser *parser = new Parser(toExecute);
	if (parser->parse()) {
		parser->postProcess();

		Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
		toExecute = evaluator->evaluate();
		cout << "-> " << toExecute << endl;
	}

	assert(strcmp(toExecute, "x2") == 0);
}

void Tester::testNumericOperations() {
	cout << "___________Testing Numeric Operations___________" << endl;
	string command("(10 + (10 - 1))");
	cout << "> " << command << endl;

	AliasManager *aliasManager = new AliasManager();
	aliasManager->consult("../files/prelude.alias");
	Translator *translator = new Translator(aliasManager);
	command = translator->translate(command);

	cout << "> " << command << endl;

	char *toExecute = (char*) malloc(strlen(command.c_str()) + 1);
	strcpy(toExecute, command.c_str());

	Parser *parser = new Parser(toExecute);
	if (parser->parse()) {
		parser->postProcess();

		ChurchNumerator *numerator = new ChurchNumerator(parser->syntaxTree, aliasManager);
		numerator->enchurch();

		Evaluator *evaluator = new Evaluator(parser->syntaxTree, aliasManager);
		free(evaluator->evaluate());
		numerator->dechurch();

		toExecute = parser->syntaxTree->toCommand();
		assert(strcmp(toExecute, "19") == 0);
	} else
		assert(!true);
}

void Tester::testTermConstruction() {
	cout << "___________Testing Term Construction___________" << endl;
	vector<string> lists;
	lists.push_back("[]");
	lists.push_back("([])");
	lists.push_back("[1]");
	lists.push_back("([1])");
	lists.push_back("[1,  2]");

	for (int i = 0; i < (int) lists.size(); i++) {
		string list = lists[i];
		cout << list << " -> ";
		cout << ListManager::translate(list) << endl;
		cout << "================" << endl;
	}
}

void Tester::testListChecker() {
	cout << "___________Testing List Checker___________" << endl;
	vector<string> lists;
	lists.push_back("(\\t. ((t true) true))");
	lists.push_back("(\\t. ((t false) (\\t. ((t 1) (\\t. ((t true) true))))))");
	lists.push_back("(\\t. ((t false) (\\t. ((t 1) (\\t. ((t false) (\\t. ((t 2) (\\t. ((t true) true))))))))))");
	lists.push_back("(\\t. ((t false) (\\t. ((t 2) (\\t. ((t false) (\\t. ((t 4) (\\t. ((t true) true))))))))))");
	lists.push_back("(\\t. ((t false) (\\t. ((t (\\t. ((t true) true))) (\\t. ((t true) true))))))");

	for (int i = 0; i < (int) lists.size(); i++) {
		string list = lists[i];
		assert(ListManager::isList(list));
	}
}

void Tester::testPrintList() {
	cout << "___________Testing List Printer___________" << endl;
	vector<string> lists;
	lists.push_back("(\\t. ((t true) true))");
	lists.push_back("(\\t. ((t false) (\\t. ((t 1) (\\t. ((t true) true))))))");
	lists.push_back("(\\t. ((t false) (\\t. ((t 1) (\\t. ((t false) (\\t. ((t 2) (\\t. ((t true) true))))))))))");
	lists.push_back("(\\t. ((t false) (\\t. ((t 2) (\\t. ((t false) (\\t. ((t 4) (\\t. ((t true) true))))))))))");

	for (int i = 0; i < (int) lists.size(); i++) {
		string list = lists[i];
		assert(ListManager::isList(list));
		cout << list << " -> " << ListManager::getPrintForm(list) << endl;
		cout << "================" << endl;
	}
}

void Tester::globalTest() {
	this->testParserErrors();
	this->testEvaluator();
	this->testAliasing();
	this->testNumericOperations();
	this->testListChecker();
	this->testTermConstruction();
	this->testPrintList();
}
