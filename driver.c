/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <stdio.h>
#include "loader.h"
#include "executor.h"
#include "variable_map.h"

int main(int argc, char * argv[])
{
	simple_file_t source;
	simple_variable_t variable_map[26];
	
	/* Check that there are exactly two command line arguments (the name of the program and
	   and name of the Simple file). */
	if (argc != 2)
	{
		printf("Please re-run the program and provide the name of the file to execute as a command-line argument.\n");
		return 1;
	}
	
	simple_initialize_variable_map(variable_map);
	source = simple_load_file(argv[1]);
	simple_execute(&source, variable_map);
	
	return 0;
}
