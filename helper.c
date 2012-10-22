/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <string.h>
#include "helper.h"

int is_integer(char * str)
{
	/* Use the is_number function, which will check that str represents a _positive_ integer. */

	if (*str == '-' && strlen(str) > 1)
	{
		return is_number(str + 1);
	}
	else
	{
		return is_number(str);
	}
}

int is_number(char * str)
{
	for (; *str != '\0'; ++str)
	{
		if (*str < '0' || *str > '9')
			return 0;
	}
	
	return 1;
}

long count_occurrences(char * buffer, char target, long size)
{
	/* Be careful since the buffer may not be null-terminated. */
	
	long count = 0;
	long i = 0;
	
	for (; i < size; ++i, ++buffer)
	{
		if (*buffer == target)
			++count;
	}
	
	return count;
}

void change_occurrences(char * buffer, char start, char end, long size)
{
	/* Be careful since the buffer may not be null-terminated. */
	
	long i = 0;
	for (; i < size; ++i, ++buffer)
	{
		if (*buffer == start)
			*buffer = end;
	}
}
