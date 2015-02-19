/*
 * ListManager.cpp
 *
 *  Created on: 17 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#include "ListManager.h"
#include <assert.h>
#include <stdio.h>
#include <vector>

ListManager::ListManager() {
}

ListManager::~ListManager() {
}

string ListManager::translate(string term) {
	int leftIndex = term.find('[');
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
			if (interior[i] == 32) i++;
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

		for (int i = 0; i < (int) items.size() - 1; i++) {
			string toInsert("(cons ");
			toInsert += items[i];
			toInsert += ") ())";

			list.insert(insertionPoint, toInsert);
			insertionPoint += 9 + items[i].size();
		}
		string toInsert("(cons ");
		toInsert += items[items.size() - 1];
		toInsert += ") ";
		toInsert += "nil";
		toInsert += ")";

		list.insert(insertionPoint, toInsert);

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
