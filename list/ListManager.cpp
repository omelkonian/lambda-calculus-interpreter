/*
 * ListManager.cpp
 *
 *  Created on: 17 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#include "ListManager.h"
#include "../error_handler/AutoCorrector.h"
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <iostream>

ListManager::ListManager() {
}

ListManager::~ListManager() {
}

string ListManager::translate(string term) {\
	int leftIndex = term.find_first_of('[');

	while (leftIndex != (int) term.npos) {
		int rightIndex = term.find(']');
		assert(rightIndex != (int) term.npos); // TODO Error Handling

		if (rightIndex == leftIndex + 1) {
			term.replace(leftIndex, 2, "nil");
			leftIndex = term.find('[');
			continue;
		}

		string interior = term.substr(leftIndex + 1, rightIndex - leftIndex - 1);

		vector<string> items;
		int i = 0;
		string item;
		while (interior[i] != ',' && i < (int) interior.size()) {
			item += interior[i++];
			if (interior[i] == ',' || i == (int) interior.size()) {
				items.push_back(item);
				item.clear();
				i++;
			}
		}

		// Construct term
		string list = "()";
		int insertionPoint = 1;

		for (int i = 0; i < (int) items.size(); i++) {
			string toInsert("(cons ");
			toInsert += items[i];
			if (i != (int) items.size() - 1)
				toInsert += ") ()";
			else
				toInsert += ") nil";

			list.insert(insertionPoint, toInsert);

			insertionPoint += 9 + items[i].size();
		}
		term.replace(leftIndex, rightIndex - leftIndex + 1, list);

		leftIndex = term.find('[');
	}
	return term;
}

string ListManager::deTranslate(string term) {
	if (term.compare("nil") == 0)
		return "[]";

	vector<string> items;
	int index = term.find("cons");
	while (index != (int) term.npos) {
		index += 4;
		string item;
		while (term[index] == 32)
			index++;
		while (term[index] != 32 && term[index] != ')' ) {
			item += term[index++];
		}
		items.push_back(item);

		index = term.find("cons", index);
	}

	string ret("[");
	for (int i = 0; i < (int) items.size() - 1; i++) {
		ret += items[i];
		ret += ", ";
	}
	ret += items[items.size() - 1];
	ret += ']';
	return ret;
}

bool ListManager::isList(string term) {
	int index = term.size() - 1;
	while (term[index] == ')') index--;
	if (index < 18) return false;
	return ((term.substr(index - 18, 19)).compare("(\\t. ((t true) true") == 0);
}

string ListManager::getPrintForm(string term) {
	vector<string> items;

	int getPoint = 9;
	string toCheck(term.substr(getPoint, 4));
	while (toCheck.compare("true") != 0) {
		// Get item
		string item;
		getPoint += 16;
		if (term[getPoint] == '(') {
			int leftIndex = getPoint;
			int rightIndex = AutoCorrector::getClosingPar(term, getPoint);
			int itemSize = rightIndex - leftIndex - 1;
			item = term.substr(leftIndex + 1, itemSize);
			getPoint += itemSize + 2;
		}
		else {
			while (term[getPoint] != ')')
				item += term[getPoint++];
		}

		items.push_back(item);
		getPoint += 11;
		toCheck = term.substr(getPoint, 4);
	}

	string list("[");
	for (int i = 0; i < (int) items.size(); i++) {
		list += items[i];
		list += (i != (int)items.size() - 1) ? ", " : "";
	}
	list += ']';
	return list;
}
