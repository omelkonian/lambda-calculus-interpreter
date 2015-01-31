/*
 * Tester.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#ifndef TESTS_TESTER_H_
#define TESTS_TESTER_H_

/**
 * Class for testing separate modules of the interpreter.
 * RT:= Real Time
 */
class Tester {
public:
	Tester();
	virtual ~Tester();

	void testScannerErrorsRT();
	void testScannerRT();
	void testParserRT();
	void testParserErrors();
	void testParser();
	void testAST();
	void testCalculator();
};

#endif /* TESTS_TESTER_H_ */
