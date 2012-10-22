/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void simple_error(enum SIMPLE_ERROR type, simple_line_t * line, const char * details)
{
	fprintf(stderr, "--ERROR--\n");
	fprintf(stderr, "Type: ");
	
	/* Add a new case if you add a new error type. */
	switch(type)
	{
		case SYNTAX_ERROR:
			fprintf(stderr, "syntax error.\n");
			break;
		
		case INVALID_LINE_NUMBERING:
			fprintf(stderr, "invalid line numbering (line numbers should form an increasing sequence of positive integers).\n");
			break;
		
		case UNRECOGNIZED_COMMAND:
			fprintf(stderr, "unrecognized command.\n");
			break;
		
		case INVALID_VARIABLE_NAME:
			fprintf(stderr, "invalid variable name (variable names must consist of precisely one lower-case letter).\n");
			break;
		
		case UNDECLARED_VARIABLE:
			fprintf(stderr, "encountered an undeclared/uninitialized variable.\n");
			break;
		
		default:
		case MYSTERY:
			fprintf(stderr, "an error of mysterious and unidentifiable origin has occurred.\n");
			break;
	}
	
	/* If a line structure is provided, print the line number. */
	if (line != NULL)
	{
		fprintf(stderr, "Line number: %d\n", line->number);
	}
	
	/* Finally, print details if provided. */
	if (details != NULL)
	{
		fprintf(stderr, "Details: %s\n", details);
	}
	
	/* Terminate the program. */
	exit(1);
}
