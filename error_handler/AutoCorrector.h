/*
 * AutoCorrector.h
 *
 *  Created on: Feb 1, 2015
 *      Author: Orestis Melkonian
 */

#ifndef ERROR_HANDLER_AUTOCORRECTOR_H_
#define ERROR_HANDLER_AUTOCORRECTOR_H_

class AutoCorrector {
	char *command;
public:
	AutoCorrector(char *command);
	virtual ~AutoCorrector();

	char* autoCorrect();

private:
	void insertAt(int position, char symbol);
	char getFirstChar();
	int getFirstCharPos();
	char getLastChar();
	int getLastCharPos();
};

#endif /* ERROR_HANDLER_AUTOCORRECTOR_H_ */
