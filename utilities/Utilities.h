/*
 * Utilities.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#ifndef UTILITIES_UTILITIES_H_
#define UTILITIES_UTILITIES_H_

#include "Range.h"

class Utilities {
public:
	Utilities();
	virtual ~Utilities();

	/*
	 * STRING OPERATIONS
	 */
	static char* insertAt(char *command, char *toInsert, int position);
	static char* remove(char *command, Range *range);
};

#endif /* UTILITIES_UTILITIES_H_ */
