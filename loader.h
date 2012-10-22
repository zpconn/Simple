/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef LOADER_H
#define LOADER_H

/* Notes:

   The loader is responsible for reading the original source file, breaking it into individual lines, and
   performing a minimal amount of parsing in order to construct for each line an instance of the
   simple_line_t struct. It also checks to see that the line numbers provided in the source file are valid,
   i.e., that they form an increasing sequence of positive integers.

   The result of the loading phase is an array of pointers to simple_line_t objects. At this point
   each line will be executed in sequence, and appropriate dispatching will be performed to make the correct
   code handle the correct type of command. */

/* Numeric identifiers for the different available commands. The NO_MATCH type is used to signal that something that
   was supposed to represent a command didn't. */
enum SIMPLE_COMMAND
{
	REM, INPUT, LET, PRINT, GOTO, IF, END, NO_MATCH
};

/* This contains a minimally parsed representation of a single line of code in the Simple file. */
typedef struct simple_line
{
	/* The number corresponding to this line. This is the number provided by the Simple programmer; it is
	   not necessarily the actual line number (e.g., the fifth line may have number 30). All that is required is
	   that these numbers form an increasing sequence of positive integers. */
	int number;
	
	/* Represents the type of command this line contains. The interpreter will perform a dispatch based on
	   this value. */
	enum SIMPLE_COMMAND command;
	
	/* This is the rest of the line AFTER the command itself. So if the entire line is
			50 if i == z goto 80
	   then number will be 50, command will be IF, and rest will be the string "i == z goto 80". */
	char * rest;
} simple_line_t;

/* This simply stores an array of lines plus the number of lines. */
typedef struct simple_file
{
	simple_line_t ** lines;
	long line_count;
	
	/* This is used by the executor module. It represents the index in the array lines of the next line of
	   code for the executor to execute. This is stored here so that commands like goto can cause
	   drastic changes to the flow of execution. */
	int next_line; 
} simple_file_t;

/* The following four functions are helpers used by simple_parse_line and simple_load_file. */

/* Determines the number of occurrences of target in buffer. */
long count_occurrences(char * buffer, char target, long size);

/* Changes all occurrences of start to end in string buffer of length size. */
void change_occurrences(char * buffer, char start, char end, long size);

/* Takes a string that is supposed to represent a command and finds the corresponding enumeration value.
   Returns NO_MATCH if no match is found. */
enum SIMPLE_COMMAND determine_command(char * command_str);

/* This function is used by simple_load_file. It takes an instance of simple_line_t with the entire line stored in the
   'rest' member. Using this it fills in all the data members of the line structure appropriately. */
void simple_parse_line(simple_line_t * line);

/* This is the main function of the loader. It takes the name of a Simple file and returns an array of
   pointers to simple_line_t objects (stored in a simple_file_t object). 

   It can throw two errors: invalid line numbering and unrecognized command. 

   Since the simple_file_t structure is very small, it's just passed around by value here to avoid the hassle of
   allocating memory for these small structs. */
simple_file_t simple_load_file(char * filename);

#endif
