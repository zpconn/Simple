/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef ERROR_H
#define ERROR_H

#include "loader.h"

/* Notes:

   This error module is just here to make error handling a little simpler and less tedious. */

/* Contains symbolic constants for various errors that can occur. Feel free to add more error types as you see fit;
   add appropriate functionality to simple_error() after doing so. */
enum SIMPLE_ERROR
{
	SYNTAX_ERROR, INVALID_LINE_NUMBERING, UNRECOGNIZED_COMMAND, INVALID_VARIABLE_NAME, UNDECLARED_VARIABLE, MYSTERY
};

/* This prints a standard error message based on the error type, shows the details if provided, and 
   terminates the program. If a pointer to the line structure is not available, pass NULL. */
void simple_error(enum SIMPLE_ERROR type, simple_line_t * line, const char * details);

#endif
