/*
 * Range.h
 *
 *  Created on: Feb 5, 2015
 *      Author: Orestis Melkonian
 */

#ifndef UTILITIES_RANGE_H_
#define UTILITIES_RANGE_H_

class Range {
public:
	int min;
	int max;

	Range(int min, int max);
	virtual ~Range();

	void print();
};

#endif /* UTILITIES_RANGE_H_ */
