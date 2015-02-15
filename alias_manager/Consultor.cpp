/*
 * Consultor.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#include "Consultor.h"
#include "../defines.h"
#include "../parser/Parser.h"

#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#include <assert.h>

Consultor::Consultor(const char *filename) {
	this->filename = filename;
}

Consultor::~Consultor() {
}

void Consultor::getStatements(AliasManager *aliasManager) {
	ifstream file(this->filename);
	vector<char*> aliases;
	vector<char*> terms;

	vector<char*> operators;
	vector<char*> opTerms;

	bool succeeded = true;
	if (file.is_open()) {
		char data[MAX_COMMAND_LENGTH];

		string line;
		int lineCount = 0;
		while (getline(file, line)) {
			lineCount++;
			stringstream lineStream(line);

			if (line.empty() || line[0] == '%' || line[0] == 13)
				continue;

			char *alias = (char*) malloc(MAX_VARIABLE_LENGTH);
			lineStream >> alias;
			lineStream >> data;
			assert(strcmp(data, ":=") == 0);

			char *term = (char*) malloc(MAX_COMMAND_LENGTH);

			int cur = 0;
			char ch = lineStream.get();
			ch = lineStream.get();

			while (ch != ';') {
				term[cur++] = ch;
				ch = lineStream.get();
			}
			term[cur] = '\0';

			bool isValidTerm = this->checkTerm(term);
			if (!isValidTerm) {
				cout << "ERROR: Term of " << alias << " is syntactically wrong. (Line " << lineCount << ")" << endl;
				succeeded = false;
				break;
			}

			// If recursion occurs, edit the command using the Y-combinator.
			string variable(alias);
			string termStr(term);
			if (this->hasRecursion(variable, termStr)) {
				cout << alias << "HAS RECURSION!" << endl;
				string finalTerm("(");
				string Y_combinator("(\\f. ((\\x. (f (x x))) (\\x. (f (x x)))))");
				finalTerm.append(Y_combinator);
				finalTerm.append(" (\\f. ");
				termStr = this->ReplaceString(termStr, alias, "f");
				finalTerm.append(termStr);
				finalTerm.append("))");

				term = (char*) realloc(term, strlen(finalTerm.c_str()) + 1);
				strcpy(term, finalTerm.c_str());
			}

			if (alias[0] == '&') {
				for (int i = 0; i < (int)strlen(alias); i++)
					alias[i] = alias[i + 1];
				operators.push_back(alias);
				opTerms.push_back(term);
			} else {
				aliases.push_back(alias);
				terms.push_back(term);
			}
		}
	} else
		cout << "ERROR: Unable to open " << file << endl;

	if (succeeded) {
		// Update Alias Manager
		for (int i = 0; i < (int) aliases.size(); i++) {
			aliasManager->addAlias(string(terms[i]), string(aliases[i]));
			free(terms[i]);
			free(aliases[i]);
		}
		for (int i = 0; i < (int) operators.size(); i++) {
			aliasManager->addOperator(string(opTerms[i]), string(operators[i]));
			free(opTerms[i]);
			free(operators[i]);
		}
	} else {
		// Free memory
		for (int i = 0; i < (int) aliases.size(); i++) {
			free(aliases[i]);
			free(terms[i]);
		}
		for (int i = 0; i < (int) operators.size(); i++) {
			free(operators[i]);
			free(opTerms[i]);
		}
	}
}

bool Consultor::checkTerm(char* term) {
	char *surround = (char*) malloc(strlen(term) + 3);
	int i = 0;
	surround[i] = '(';
	while (i < (int)strlen(term)) surround[i + 1] = term[i++];
	surround[++i] = ')';
	surround[++i] = '\0';

	Parser *parser = new Parser(surround);
	bool ret = parser->parse();
	delete parser;
	free(surround);
	return ret;
}

bool Consultor::hasRecursion(string variable, string term) {
	int index = term.find(variable);
	if (index != (int) term.npos) {
		bool spaceBefore = (index > 0) ? (term[index - 1] == 32 || term[index - 1] == '(') : (true);
		bool spaceAfter = (index < (int)term.size()) ? (term[index + 1] == 32 || term[index + 1] == ')') : (true);
		return (spaceAfter && spaceBefore);
	}
	return false;
}

string Consultor::ReplaceString(string subject, const string& search, const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}
