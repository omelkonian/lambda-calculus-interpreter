/*
 * AutoCorrector.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: Orestis Melkonian
 */

#include "AutoCorrector.h"
#include "../scanner/Scanner.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <assert.h>

#include <iostream>

using namespace std;

AutoCorrector::AutoCorrector() {
}

AutoCorrector::~AutoCorrector() {
}
int AutoCorrector::getLeftParNo(string term) {
	int ret = 0;
	for (int i = 0; i < (int) term.size(); i++)
		if (term[i] == '(')
			ret++;
	return ret;
}

int AutoCorrector::getRightParNo(string term) {
	int ret = 0;
	for (int i = 0; i < (int) term.size(); i++)
		if (term[i] == ')')
			ret++;
	return ret;
}

string AutoCorrector::removeUnnecessaryParentheses(string term) {
	int leftParNo = AutoCorrector::getLeftParNo(term);
	int rightParNo = AutoCorrector::getRightParNo(term);

	if (leftParNo == 0 || leftParNo != rightParNo || leftParNo == 1)
		return term;

	int leftIndex = term.find_first_of('(');
	int rightIndex = AutoCorrector::getClosingPar(term, leftIndex);
	assert(rightIndex != -1);
	if (leftIndex == rightIndex - 1)
		return term;
	while (leftIndex != (int) term.npos) {
		bool leftIn = (term[leftIndex + 1] == '(') ? true : false;
		bool rightIn = (rightIndex - 1 == AutoCorrector::getClosingPar(term, leftIndex + 1)) ? true : false;

		bool simpleTerm1 = true;
		for (int i = leftIndex + 1; i < rightIndex; i++) {
			if (!(Scanner::isValidVarSymbol(term[i]) || Scanner::isValidDigit(term[i]) || term[i] == '[' || term[i] == ']')) {
				simpleTerm1 = false;
				break;
			}
		}

		bool list = ((term[leftIndex + 1] == '[') && (term[rightIndex - 1] == ']'));
		if (list) {
			for (int i = leftIndex + 2; i < rightIndex - 1; i++)
				if (term[i] == '[' || term[i] == ']') list = false;
		}

		bool replaced = false;
		if (leftIn && rightIn) {
			term.erase(leftIndex + 1, 1);
			term.erase(rightIndex - 1, 1);
			replaced = true;
		} else if (simpleTerm1 || list) {
			term.erase(leftIndex, 1);
			term.erase(rightIndex - 1, 1);
			replaced = true;
		}
		if (!replaced) {
			term[leftIndex] = 0;
			term[rightIndex] = 1;
		}

		leftIndex = term.find_first_of('(', leftIndex + 1);
		if (leftIndex != (int) term.npos)
			rightIndex = AutoCorrector::getClosingPar(term, leftIndex);
	}

	for (int i = 0; i < (int) term.size(); i++) {
		if (term[i] == 0) term[i] = '(';
		else if (term[i] == 1) term[i] = ')';
	}

	return term;
}

int AutoCorrector::getClosingPar(string term, int openingPar) {
	int parCount = 1;
	for (int i = openingPar + 1; i < (int) term.size(); i++) {
		if ((term[i] == ')') && (--parCount == 0))
			return i;
		else if (term[i] == '(')
			parCount++;
	}
	return -1;
}
