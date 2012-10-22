/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <stdio.h>
#include "error.h"
#include "executor.h"
#include "command_parser.h"

/* Despite its central role, this function is fairly simple. It works by repeatedly executing the line
   whose index is given by the next_line member of the file structure. This index is changed each time
   a line is executed; usually it is just incremented, but it can be changed arbitrarily by, e.g., the
   goto commands. next_line is just the index into the array in file, not the actual line number provided
   by the Simple programmer. 

   This ceases execution when next_line is set to a negative value. Thus, for example, the END command could
   work by just setting next_line to -1. */
void simple_execute(simple_file_t * file, simple_variable_t variable_map[26])
{
	while (file->next_line >= 0)
	{
		simple_execute_line(file, variable_map, file->next_line);
	}
}

void simple_execute_line(simple_file_t * file, simple_variable_t variable_map[26], int line_index)
{
	simple_line_t * line = NULL;
	
	/* First make sure line_index is within a valid range. */
	if (line_index < 0 || line_index >= file->line_count)
	{
		simple_error(MYSTERY, NULL, "the program flow is trying to go beyond available lines; is an 'end' command missing?");
	}
	
	line = file->lines[line_index];
	
	/* Dispatch control based on the command type. The code for each of these cases
	   should ideally be implemetned in separate files; otherwise the overall simplicity
	   of this function would be obscured and the code could become difficult to maintain. 
	   For now, each case will increment the next_line, but in the end this should be done
	   inside the functions that will handle the cases correctly. */
	
	switch (line->command)
	{
		case REM:
			/* Nothing to do! */
			++file->next_line;
			break;
		
		case INPUT:
			input_command(line, variable_map);
			++file->next_line;
			break;
		
		case LET:
			let_command(line, variable_map);
			++file->next_line;
			break;
		
		case PRINT:
			print_command(line, variable_map);
			++file->next_line;
			break;
		
		case GOTO:
			goto_command(file, line, variable_map);
			break;
		
		case IF:
			conditional_command(file, line, variable_map);
			break;
		
		case END:
			file->next_line = -1;
			break;
		
		default:
		case NO_MATCH:
			/* Something has went wrong... */
			simple_error(MYSTERY, line, "tried to execute a line whose command is NO_MATCH");
			break;
	}
}
