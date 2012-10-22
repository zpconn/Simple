/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "loader.h"
#include "variable_map.h"

/* Notes:

	The executor is responsible for taking the results from the loader and executing the Simple program.
	It works by dispatching control based on the command types that were determined by the loader.
	The code for implementing each individual command should appear in its own header/source file pair
	for moduarlity and cleanliness. */

/* This is the core function of the interpreter that is responsible for overseeing the broad execution
   of the Simple program. It takes as input the result produced by the loader. */
void simple_execute(simple_file_t * file, simple_variable_t variable_map[26]);

/* Executes a particular line of the provided file. */
void simple_execute_line(simple_file_t * file, simple_variable_t variable_map[26], int line_index);

#endif
