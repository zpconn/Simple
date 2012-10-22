/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <string.h>
#include <ctype.h>
#include "variable_map.h"
#include "error.h"

void simple_initialize_variable_map(simple_variable_t variable_map[26])
{
	int i = 0;
	for (; i < 26; ++i)
	{
		variable_map[i].initialized = 0;
		variable_map[i].value = 0;
	}
}

void check_variable_name(char * name, simple_line_t * line)
{
	char c;

	/* Check that the length is exactly 1. */
	if (strlen(name) != 1)
	{
		simple_error(INVALID_VARIABLE_NAME, line, name);
	}
	
	/* Now check that this one character is alphabetic. */
	c = *name;
	if (!isalpha(c))
	{
		simple_error(INVALID_VARIABLE_NAME, line, name);
	}
	
	/* Finally check that this character is lower-case. */
	if (c != tolower(c))
	{
		simple_error(INVALID_VARIABLE_NAME, line, name);
	}
}

int check_variable_name_no_error(char * name)
{
	char c;
	
	if (strlen(name) != 1)
	{
		return 0;
	}
	
	c = *name;
	
	if (!isalpha(c) || c != tolower(c))
	{
		return 0;
	}
	
	return 1;
}

int simple_lookup_variable(simple_variable_t variable_map[26], char * name, simple_line_t * line)
{
	char c;
	
	/* Make sure the name is valid. If so, grab it. */
	check_variable_name(name, line);
	c = *name;
	
	/* This originally checked that variables were initialized before using them, but actually the desired
	   behavior based on the project description is that it should just set the variables to 0
	   and use them. The variables are all already set to 0, so this is why the following is commented out. */
	
	/*
	
	if (!variable_map[c - 'a'].initialized)
	{
		simple_error(UNDECLARED_VARIABLE, line, name);
	}
	
	*/
	
	/* All is well, so just return the value. */
	return variable_map[c - 'a'].value;
}

void simple_assign_variable(simple_variable_t variable_map[26], char * name, int value, simple_line_t * line)
{
	char c;
	
	/* Make sure the name is valid. If so, grab it. */
	check_variable_name(name, line);
	c = *name;
	
	/* Assign the new value and remember that this variable is initialized. */
	variable_map[c - 'a'].initialized = 1;
	variable_map[c - 'a'].value = value;
}
