/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef VARIABLE_MAP_H
#define VARIABLE_MAP_H

#include "loader.h"

/* This struct represents a variable. The name isn't stored since this can be determined by the variable map declared
   below. */
typedef struct simple_variable
{
	int initialized;
	int value;
} simple_variable_t;

/* Sets the 'value' of each variable to 0 and 'initialized' to 0 (false). */
void simple_initialize_variable_map(simple_variable_t variable_map[26]);

/* This simply checks that the string 'name' contains precisely one lower-case alphabetic character, so that 'name'
   actually represents a valid variable name. This takes the 'line' parameter for error-reporting purposes. On success,
   nothing happens. On failure, the error is reported and the program terminated. */
void check_variable_name(char * name, simple_line_t * line);

/* This is the same as the previous function except it does not throw any errors. It returns 1 for a valid variable name
   and 0 otherwise. */
int check_variable_name_no_error(char * name);

/* This takes a string representing a variable name and attempts to lookup the variable's value
   in the provided variable map reference. On success it returns the value. On failure, it
   reports the error with simple_error() [using information from 'line'], thereby terminating
   the program. This streamlines the lookup of variables in outside code together with
   error reporting. */
int simple_lookup_variable(simple_variable_t variable_map[26], char * name, simple_line_t * line);

/* This behaves just like the previous function except it attempts to assign a numeric value to a variable
   instead of lookup the value of an already initialized variable. */
void simple_assign_variable(simple_variable_t variable_map[26], char * name, int value, simple_line_t * line);

#endif
