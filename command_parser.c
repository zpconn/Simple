/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command_parser.h"
#include "error.h"
#include "expression.h"
#include "helper.h"

int find_line_index(simple_file_t * file, int line_number)
{
	int low = 0;
	int high = file->line_count - 1;
	int middle;
	int compare_result;

	while (low <= high)
	{
		middle = (low + high) / 2;
		compare_result = line_number < file->lines[middle]->number;

		if (line_number == file->lines[middle]->number)
			return middle;
		else if (line_number < file->lines[middle]->number)
			high = middle - 1;
		else
			low = middle + 1;
	}

	/* If we made it this far, the line wasn't found. */
	return -1;
}

enum SIMPLE_CONDITIONAL determine_conditional(char * string)
{
	if (strcmp(string, "<") == 0)
		return LESS_THAN_STRICT;
	else if (strcmp(string, "<=") == 0)
		return LESS_THAN;
	else if (strcmp(string, "==") == 0)
		return EQUALS;
	else if (strcmp(string, ">=") == 0)
		return GREATER_THAN;
	else if (strcmp(string, ">") == 0)
		return GREATER_THAN_STRICT;
	else
		return INVALID_CONDITIONAL;
}

int perform_comparison(simple_line_t * line, int a, int b, enum SIMPLE_CONDITIONAL conditional)
{
	switch (conditional)
	{
		case LESS_THAN_STRICT:
			return a < b;
		
		case LESS_THAN:
			return a <= b;
		
		case EQUALS:
			return a == b;
		
		case GREATER_THAN:
			return a >= b;
		
		case GREATER_THAN_STRICT:
			return a > b;
		
		default:
		case INVALID_CONDITIONAL:
			simple_error(MYSTERY, line, "somehow tried to evaluate an INVALID_CONDITIONAL comparison");
			return 0;
	}
}

void input_command(simple_line_t * line, simple_variable_t variable_map[26])
{
	int new_value = 0;

	/* Check that the provided name is a valid variable name. */
	check_variable_name(line->rest, line);
	
	/* Query the user for a value. */
	printf("? ");
	
	/* IIRC, we are allowed to assume the input is syntactically valid. Hence, it is safe to use scanf in this way. 
	   As a future note, though, it may be beneficial to come back and add in error checking. */
	scanf("%d", &new_value);
	
	/* Now assign the variable its new value. */
	simple_assign_variable(variable_map, line->rest, new_value, line);
}

void conditional_command(simple_file_t * file, simple_line_t * line, simple_variable_t variable_map[26])
{
	int a, b;
	int i = 0;
	char * token = NULL;
	char * tokens[5] = {NULL, NULL, NULL, NULL, NULL};
	enum SIMPLE_CONDITIONAL conditional = NO_MATCH;
	int comparison_value = 0;
	int goto_destination = -1;
	
	/* We will use strtok, which modifies the line, so we copy the line first. */
	char * copy = malloc(strlen(line->rest) + 1);
	strcpy(copy, line->rest);
	
	/* The conditional should have a strict syntax: [a] [compare] [b] goto [c] where a and b are variable names or integers, c is a 
	   positive integer, and compare is one of <, <=, ==, >=, >. */
	
	/* The first task is to break the string into the five pieces a, compare, b, 'goto', and c. */

	token = strtok(copy, " ");
	
	/* Simulate the first iteration. */
	tokens[0] = token;
	++i;
	
	while (i < 5 && token != NULL)
	{
		token = strtok(NULL, " ");
		tokens[i] = token;
		++i;
	}
	
	/* Make sure nothing went wrong. If tokens[4] is NULL, then we weren't able to read enough tokens, and if we can read *another* token, then
	   there are too many tokens. */
	if (tokens[4] == NULL || strtok(NULL, " ") != NULL)
	{
		simple_error(SYNTAX_ERROR, line, "the conditional statement isn't well-formed");
	}
	
	/* We now check the tokens in sequence. */
	
	/* tokens[0] should represent a variable name or an integer. */	
	if (check_variable_name_no_error(tokens[0]))
	{
		a = simple_lookup_variable(variable_map, tokens[0], line);
	}
	else
	{
		/* tokens[0] should be an integer, not necessarily positive. First check that this is the case. */
		if (!is_integer(tokens[0]))
		{
			simple_error(SYNTAX_ERROR, line, "the operands of the comparison should be variable names or integer literals");
		}
		
		a = atoi(tokens[0]);
	}
	
	/* tokens[1] should represent the conditional. */
	conditional = determine_conditional(tokens[1]);
	
	if (conditional == INVALID_CONDITIONAL)
	{
		simple_error(SYNTAX_ERROR, line, "invalid or unrecognizable conditional provided; valid choices are <, <=, ==, >=, and >");
	}
	
	/* tokens[2] should represent a variable name or an integer. */	
	if (check_variable_name_no_error(tokens[2]))
	{
		b = simple_lookup_variable(variable_map, tokens[2], line);
	}
	else
	{
		/* tokens[0] should be an integer, not necessarily positive. First check that this is the case. */
		if (!is_integer(tokens[2]))
		{
			simple_error(SYNTAX_ERROR, line, "the operands of the comparison should be variable names or integer literals");
		}
		
		b = atoi(tokens[2]);
	}
	
	/* tokens[3] should literally be "goto". */
	
	if (strcmp(tokens[3], "goto") != 0)
	{
		simple_error(SYNTAX_ERROR, line, "the conditional statement isn't well-formed; is there a typo in the 'goto' token?");
	}
	
	/* tokens[4] should be a positive integer. */
	if (!is_number(tokens[4]))
	{
		simple_error(SYNTAX_ERROR, line, "the final token of the conditional should be a positive integer indicating the line number to jump to");
	}
	
	/* Evaluate the conditional. */
	comparison_value = perform_comparison(line, a, b, conditional);
	
	/* If true, goto the destination. */
	if (comparison_value)
	{
		goto_destination = find_line_index(file, atoi(tokens[4]));

		if (goto_destination < 0)
		{
			simple_error(SYNTAX_ERROR, line, "no line with the provided number is present in the file");
		}

		/* Now set our destination. */
		file->next_line = goto_destination;
	}
	else
	{
		/* Otherwise, just go to the next line. */
		++file->next_line;
	}
}

void print_command(simple_line_t * line, simple_variable_t variable_map[26])
{
	/* Pretty much everything is handled by the simple_lookup_variable function. */
	printf("%s = %d\n", line->rest, simple_lookup_variable(variable_map, line->rest, line));
}

void goto_command(simple_file_t * file, simple_line_t * line, simple_variable_t variable_map[26])
{
	int goto_destination = -1;
	
	/* Use the is_number function from the loader module to check that a positive integer has been provided. */
	if (!is_number(line->rest))
	{
		simple_error(SYNTAX_ERROR, line, "the goto destination is not a positive integer");
	}
	
	goto_destination = find_line_index(file, atoi(line->rest));
	
	if (goto_destination < 0)
	{
		simple_error(SYNTAX_ERROR, line, "no line with the provided number is present in the file");
	}
	
	/* Now set our destination. */
	file->next_line = goto_destination;
}

void let_command(simple_line_t * line, simple_variable_t variable_map[26])
{
	char * tokens[3] = {NULL, NULL, NULL};
	int result = 0;
	char * str = malloc(strlen(line->rest) + 1);
	strcpy(str, line->rest);
	
	/* The first token should be a variable name. */
	
	tokens[0] = strtok(str, " ");
	
	check_variable_name(tokens[0], line);
	
	/* The second token should literally be "=". */
	
	tokens[1] = strtok(NULL, " ");
	
	if (strcmp(tokens[1], "=") != 0)
	{
		simple_error(SYNTAX_ERROR, line, "invalid syntax for assignment statement; are you missing an = sign?");
	}
	
	/* The third "token" is the entire expression to evaluate. */
	
	tokens[2] = strchr(strchr(str, 0) + 1, 0) + 1;
	
	if (tokens[2] == NULL || strcmp(tokens[2], "\n") == 0 || strcmp(tokens[2], "\0") == 0)
	{
		simple_error(SYNTAX_ERROR, line, "it appears you're missing an expression to assign to the provided variable");
	}
	
	result = exp_evaluate(line, tokens[2], variable_map);
	simple_assign_variable(variable_map, tokens[0], result, line);
}
