/*
 * Tester.h
 *
 *  Created on: Jan 28, 2015
 *      Author: Orestis Melkonian
 */

#ifndef TESTS_TESTER_H_
#define TESTS_TESTER_H_

/**
 * 	Class for testing separate modules of the interpreter.
 * 	RT:= Real Time
 */
class Tester {
public:
	Tester();
	virtual ~Tester();

	// PARSER
	void testParserErrors();

	// EVALUATOR
	void testEvaluator();

	// ALIAS MANAGER
	void testAliasing();

	// CHURCH ENCODING
	void testNumericOperations();

	// LIST
	void testTermConstruction();
	void testListChecker();
	void testPrintList();


	void globalTest();
};

#endif /* TESTS_TESTER_H_ */
