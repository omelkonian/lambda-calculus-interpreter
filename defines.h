/*
 * defines.h
 *
 *  Created on: Jan 27, 2015
 *      Author: Orestis Melkonian
 */

#ifndef LCI_DEFINES_H_
#define LCI_DEFINES_H_

// LIMITS
#define MAX_COMMAND_LENGTH 100000
#define MAX_VARIABLE_LENGTH 100
#define MAX_NUMBER_DIGITS 10
#define MAX_NUMBER 500
#define MAX_ERROR_MESSAGE_LENGTH 500

// EXECUTION
extern bool EAGER_EVALUATION;
extern bool TRACE;
extern bool NON_STOP;
extern bool TIME;

extern bool PURE_TERM;

// DEBUGGING
extern bool DEBUG;

// ERROR-HANDLING
extern bool ERROR_FOUND;

#endif /* LCI_DEFINES_H_ */
