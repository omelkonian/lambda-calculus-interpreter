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
#include <string.h>

using namespace std;

TokenValue::TokenValue(TokenValue* copy) {
	this->type = copy->type;
	if (copy->type == STRING) {
		this->value.string = (char*) malloc(strlen((const char*)copy->value.string));
		strcpy(this->value.string, copy->value.string);
	} else
		this->value = copy->value;
}

TokenValue::TokenValue(ValueType type, void *val) {
this->type = type;
switch (type) {
case STRING:
	this->value.string = (char*) val;
	break;
case CHAR:
	this->value.character = *(char*) val;
	break;
case INTEGER:
	this->value.number = *(int*) val;
	break;
default:
	break;
}
}

TokenValue::~TokenValue() {
if (this->type == STRING)
	free(this->value.string);
}

void TokenValue::print() {
cout << "Value: ";
if (this->type == STRING) {
	cout << value.string << endl;
} else if (this->type == CHAR)
	cout << value.character << endl;
else if (this->type == INTEGER)
	cout << value.number << endl;
else
	cout << "None" << endl;
}

void TokenValue::print(int indent) {
for (int i = 0; i < indent; i++)
	cout << " ";
if (this->type == STRING) {
	cout << value.string << endl;
} else if (this->type == CHAR)
	cout << value.character << endl;
else if (this->type == INTEGER)
	cout << value.number << endl;
else
	cout << "None" << endl;
}

