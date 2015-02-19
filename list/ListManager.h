/*
 * ListManager.h
 *
 *  Created on: 17 ÷Â‚ 2015
 *      Author: Orestis Melkonian
 */

#ifndef LIST_LISTMANAGER_H_
#define LIST_LISTMANAGER_H_

#include <string>
using namespace std;

class ListManager {
public:
	ListManager();
	virtual ~ListManager();

	string translate(string term);

	string deTranslate(string term);
};

#endif /* LIST_LISTMANAGER_H_ */
