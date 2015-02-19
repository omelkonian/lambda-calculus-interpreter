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

	// SCANNER
	void testScannerRT();

	// PARSER
	void testParserRT();
	void testParser();

	// CALCULATOR
	void testCalculator();

	// EVALUATOR
	void testEvaluatorRT();
	void testEvaluator();

	// ERROR-CHECKING
	void testScannerErrorsRT();
	void testParserErrors();
	void testAutocorrectorRT();

	// TREE OPERATIONS
	void testAST();
	void testVariablePool();

	// STRING OPERATIONS
	void testStringManipulation();

	// ALIAS MANAGER
	void testAliasing();

	// CHURCH ENCODING
	void testEnchurch();
	void testDechurch();
	void testNumericOperations();

	// LIST
	void testListChecker();






	void globalTest();
};

#endif /* TESTS_TESTER_H_ */
