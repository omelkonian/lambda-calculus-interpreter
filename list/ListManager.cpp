/*
 * ListManager.cpp
 *
 *  Created on: 17 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#include "ListManager.h"
#include <assert.h>

ListManager::ListManager(AliasManager* aliasManager) {
	this->aliasManager = aliasManager;
}

ListManager::~ListManager() {
}

string ListManager::translate(string term) {
	int leftIndex = term.find('[');
	while (leftIndex != (int) term.npos) {
		int rightIndex = term.find(']');
		assert(rightIndex != (int) term.npos);

		string interior = term.substr(leftIndex + 1, rightIndex - leftIndex - 1);
		cout << "interior: " << interior << endl;

		vector<string> items;
		int i = 0;
		string item;
		while (interior[i] != ',' && i < (int) interior.size()) {
			item += interior[i++];
			cout << "item: " << item << endl;
			if (interior[i] == ',' || i == (int) interior.size()) {
				items.push_back(item);
				item.clear();
				i++;
			}
		}

		for (int i = 0; i < (int) items.size(); i++)
			cout << "item " << i << ": " << items[i] << endl;

//		leftIndex = term.find('[');
		leftIndex = (int)term.npos;
	}
}
