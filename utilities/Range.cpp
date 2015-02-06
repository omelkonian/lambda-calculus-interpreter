/*
 * Range.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#include "Range.h"
#include <assert.h>

#include <iostream>
using namespace std;

Range::Range(int min, int max) {
	assert(min <= max);
	this->min = min;
	this->max = max;
}

Range::~Range() {
}

void Range::print() {
	cout << "(" << this->min << ", " << this->max << ")" << endl;
}

