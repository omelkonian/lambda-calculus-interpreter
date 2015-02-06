/*
 * Consultor.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#include "Consultor.h"
#include "../defines.h"

#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#include <assert.h>

Consultor::Consultor(char *filename) {
	this->filename = filename;
}

Consultor::~Consultor() {
	// TODO Auto-generated destructor stub
}

void Consultor::getStatements(AliasManager *aliasManager) {
	ifstream file(this->filename);
	vector<char*> aliases;
	vector<char*> terms;

	bool succeeded = true;
	if (file.is_open()) {
		cout << "Successfully opened " << this->filename << endl;
		char data[MAX_COMMAND_LENGTH];

		string line;
		int lineCount = 1;
		while (getline(file, line)) {
			stringstream lineStream(line);

			if (line[0] == '%')
				continue;

			char *alias = (char*) malloc(MAX_VARIABLE_LENGTH);
			lineStream >> alias;
			lineStream >> data;
			assert(strcmp(data, ":=") == 0);

			char *term = (char*) malloc(MAX_COMMAND_LENGTH);

			int cur = 0;
			char ch;
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

			// Minimize memory allocation.
			alias = (char*) realloc(alias, strlen(alias) + 1);
			term = (char*) realloc(term, strlen(term) + 1);

			aliases.push_back(alias);
			terms.push_back(term);

			lineCount++;
		}
	} else
		cout << "ERROR: Unable to open " << file << endl;

	if (succeeded) {
		// Update Alias Manager
		for (int i = 0; i < (int) aliases.size(); i++)
			aliasManager->addAlias(terms[i], aliases[i]);
	}
	else {
		// Free memory
		for (int i = 0; i < (int) aliases.size(); i++) {
			free(aliases[i]);
			free(terms[i]);
		}
	}
}

bool Consultor::checkTerm(char* term) {
	Parser *parser = new Parser(term);
	bool ret = parser->parse;
	delete parser;
	return ret;
}
