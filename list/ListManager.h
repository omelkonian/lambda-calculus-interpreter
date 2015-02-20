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

	static string translate(string term);
	static string deTranslate(string term);
	static bool isList(string term);
	static string getPrintForm(string term);
};

#endif /* LIST_LISTMANAGER_H_ */
