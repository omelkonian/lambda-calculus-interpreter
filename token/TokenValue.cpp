/*
 * TokenValue.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: Orestis Melkonian
 */

#include "TokenValue.h"
#include <stdio.h>
#include <iostream>
#include <malloc.h>

using namespace std;

TokenValue::TokenValue(int type, void *val) {
	this->type = type;
	switch (type) {
	// String
	case 0:
		this->value.string = (char*) val;
		break;
	// Character
	case 1:
		this->value.character = *(char*)val;
		break;
	// Integer
	case 2:
		this->value.number = *(int*)val;
		break;
	}
}

TokenValue::~TokenValue() {
	if (this->type == 0)
		free(this->value.string);
}

void TokenValue::print() {
	cout << "Value: ";
	if (this->type == 0) {
		if (this->value.string)
			cout << value.string << endl;
		else
			cout << "NULL" << endl;
	} else if (this->type == 1)
		cout << value.character << endl;
	else
		cout << value.number << endl;

}
