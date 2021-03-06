/*
 * Variable.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ALIAS_MANAGER_VARIABLE_H_
#define ALIAS_MANAGER_VARIABLE_H_

#include <stdlib.h>
#include "../utilities/Range.h"

#include <iostream>
using namespace std;

class Variable {
public:
	string name;
	Range *range;

	Variable(string name, Range *range) {
		this->name = name;
		this->range = range;
	}
	virtual ~Variable() {
		delete range;
	}

	void print() {
		cout << "Name: " << this->name << endl;
		cout << "Range: ";
		this->range->print();
	}
};

#endif /* ALIAS_MANAGER_VARIABLE_H_ */
