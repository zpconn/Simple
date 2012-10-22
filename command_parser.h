/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "loader.h"
#include "variable_map.h"

/* LESS_THAN_STRICT corresponds to <, LESS_THAN corresponds to <=, etc. */
enum SIMPLE_CONDITIONAL
{
	LESS_THAN_STRICT, LESS_THAN, EQUALS, GREATER_THAN, GREATER_THAN_STRICT, INVALID_CONDITIONAL
};

/* Uses binary search to find the index of the line with number line_number in the provided file. Returns
   -1 if the line isn't found. */
int find_line_index(simple_file_t * file, int line_number);

/* Takes a string that's supposed to represent a conditional and determines which one it does actually represent.
   This returns INVALID_CONDITIONAL if no matching conditional is found. */
enum SIMPLE_CONDITIONAL determine_conditional(char * string);

/* Returns the result of evaluating the comparisoin a [conditional] b. */
int perform_comparison(simple_line_t * line, int a, int b, enum SIMPLE_CONDITIONAL conditional);

void input_command(simple_line_t * line, simple_variable_t variable_map[26]);

void conditional_command(simple_file_t * file, simple_line_t * line, simple_variable_t variable_map[26]);

void print_command(simple_line_t * line, simple_variable_t variable_map[26]);

void goto_command(simple_file_t * file, simple_line_t * line, simple_variable_t variable_map[26]);

void let_command(simple_line_t * line, simple_variable_t variable_map[26]);

#endif
