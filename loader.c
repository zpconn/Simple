/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "loader.h"
#include "error.h"
#include "helper.h"

enum SIMPLE_COMMAND determine_command(char * command_str)
{
	if (strcmp(command_str, "rem") == 0)
		return REM;
	else if (strcmp(command_str, "input") == 0)
		return INPUT;
	else if (strcmp(command_str, "let") == 0)
		return LET;
	else if (strcmp(command_str, "print") == 0)
		return PRINT;
	else if (strcmp(command_str, "goto") == 0)
		return GOTO;
	else if (strcmp(command_str, "if") == 0)
		return IF;
	else if (strcmp(command_str, "end") == 0)
		return END;
	else
		return NO_MATCH;
}

void simple_parse_line(simple_line_t * line)
{
	/* Break the line into three pieces. The first piece should contain the line number. The second should
	   contain the command name. And the third piece should contain everything else. */
	
	/* An extremely subtle bug is possible here. The lines are technically still stored right next to one another inside
	   the large contigious buffer allocated by simple_load_file. Therefore, it's possible that if the current line has only two
	   tokens, then the following will read into the _next_ line. To avoid this, this function copies the line
	   and works with the copy. */
	
	char delimiters[2] = " \t";
	char * tokens[3] = {NULL, NULL, NULL};
	char * str = malloc(strlen(line->rest) + 1);
	strcpy(str, line->rest);
	
	tokens[0] = strtok(str, delimiters);
	
	/* Make sure this line contains something. */
	if (tokens[0] == NULL)
	{
		simple_error(SYNTAX_ERROR, NULL, "empty/blank line; is this a blank line at the end? ");
	}
	
	/* Check that the first piece contains a positive integer. */
	if (!is_number(tokens[0]))
	{
		simple_error(SYNTAX_ERROR, NULL, "line does not start with positive integer");
	}
	
	/* Save the line number. */
	line->number = atoi(tokens[0]);
	
	/* Next token! */
	tokens[1] = strtok(NULL, delimiters);
	
	/* Every line should contain at least two tokens. */
	if (tokens[1] == NULL)
	{
		simple_error(SYNTAX_ERROR, NULL, "line does not contain enough tokens to be well-defined");
	}
	
	line->command = determine_command(tokens[1]);
	
	/* It is a syntax error if there was no match. */
	if (line->command == NO_MATCH)
	{
		simple_error(UNRECOGNIZED_COMMAND, NULL, tokens[1]);
	}
	
	/* If the command is END, then we are done. Otherwise we need to store the rest of the string in line->rest. */
	
	if (line->command == END)
	{
		tokens[2] = NULL;
	}
	else
	{
		/* The spaces at the end of the first and second tokens were replaced by \0 by strtok.
		   So to find the beginning of the rest of the string, we find the second instance of \0. */
		
		/* It is here that the subtle bug mentioned above was occuring before the code was modified to make
		   a copy of line->rest into str. */
		tokens[2] = strchr(strchr(str, 0) + 1, 0) + 1;
		
		/* It's possible tokens[2] is now NULL. It is a syntax error if it is and the command is not REM. */
		if ((tokens[2] == NULL || strcmp(tokens[2], "\n") == 0 || strcmp(tokens[2], "\0") == 0) && line->command != REM)
		{
			simple_error(SYNTAX_ERROR, NULL, "a command other than 'rem' and 'end' was encountered with no arguments");
		}
	}
	
	line->rest = tokens[2];
}

/* This fairly complicated function handles the entire loading stage using the other helper functions implemented
   in this file. */
simple_file_t simple_load_file(char * filename)
{
	FILE * fp = NULL;
	long pos = 0;
	long line_count = 0;
	char * buffer = NULL;
	char * pch = NULL;
	simple_file_t file;
	int i = 0;
	int prev_line_num = -1;
	
	/* -- Phase 1: load the entire file into a single buffer. */
	
	fp = fopen(filename, "rb");
	
	if (fp == NULL)
	{
		simple_error(MYSTERY, NULL, "couldn't open file");
	}
	
	/* A trick to determine the size of the file in bytes. */
	fseek(fp, 0, SEEK_END);
	pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	buffer = malloc(pos);
	
	if (buffer == NULL)
	{
		simple_error(MYSTERY, NULL, "couldn't allocate buffer for reading file in simple_load_file");
	}
	
	fread(buffer, pos, 1, fp);
	fclose(fp);
	
	/* -- Phase 2: create an array of strings with the nth string corresponding to the nth line of the file. */
	
	line_count = count_occurrences(buffer, '\n', pos) + 1;
	file.line_count = line_count;
	file.lines = malloc(line_count * sizeof(simple_line_t *));
	
	if (file.lines == NULL)
	{
		simple_error(MYSTERY, NULL, "couldn't allocate lines array in simple_load_file");
	}
	
	/* We now change the '\n' characters to 0. This effectively partitions the buffer into strings, each
	   string representing a single line of Simple code. */
	
	change_occurrences(buffer, '\n', 0, pos);
	
	/* We will temporarily put the entire string for a given line into the rest member of that line's corresponding
	   structure. Later on the lines will be parsed and the structures filled out appropriately.*/
	
	/* Set up the first line manually. */
	file.lines[0] = malloc(sizeof(simple_line_t));
	file.lines[0]->rest = buffer;
	++i;
	
	/* Handle the rest iteratively. */
	pch = strchr(buffer, 0);
	
	while (pch != NULL && i < line_count)
	{
		file.lines[i] = malloc(sizeof(simple_line_t));
		file.lines[i]->rest = pch + 1;		
		pch = strchr(pch + 1, 0);
		++i;
	}
	
	/* -- Phase 3: parse each line. This phase will throw errors if any are encountered. */
	
	for (i = 0; i < line_count; ++i)
	{
		simple_parse_line(file.lines[i]);
	}
	
	/* Check that the line numbers are increasing. It is already known that they are positive integers if we made it here. */
	
	for (i = 0; i < line_count; ++i)
	{
		if (file.lines[i]->number <= prev_line_num)
		{
			simple_error(INVALID_LINE_NUMBERING, NULL, NULL);
		}
		
		prev_line_num = file.lines[i]->number;
	}
	
	free(buffer);
	
	/* One last data member remains uninitialized... */
	file.next_line = 0;
	
	return file;
}
