/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef HELPER_H
#define HELPER_H

/* Determines whether str represents a positive integer. */
int is_number(char * str);

/* Determines whether or not the string 'str' represents an integer, not necessarily positive. 
   Returns 1 if str does represent an integer, 0 otherwise. */
int is_integer(char * str);

/* Determines the number of occurrences of target in buffer. */
long count_occurrences(char * buffer, char target, long size);

/* Changes all occurrences of start to end in string buffer of length size. */
void change_occurrences(char * buffer, char start, char end, long size);

#endif
