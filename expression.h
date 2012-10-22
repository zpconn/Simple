/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "variable_map.h"
#include "loader.h"
#include "data_structures.h"

enum SIMPLE_OPERATOR
{
	LEFT_PARENTHESIS, RIGHT_PARENTHESIS, PLUS, MINUS, TIMES, DIVIDE, MODULUS, OP_NO_MATCH
};

/* This inserts a space to the right and left of every operator and parenthesis. It allocates a new string and returns a
   pointer to this, which must be externally freed. */
char * pad_string(char * str);

/* Determines which operator this token represents, and returns OP_NO_MATCH if the token doesn't represent an operator. */
enum SIMPLE_OPERATOR token_determine_op(char * token);

/* Evaluates the expression 'operand1 op operand2'. */
int evaluate_operator(enum SIMPLE_OPERATOR op, int operand1, int operand2);

/* Determines a numerical precedence for an operator. */
int op_precedence(enum SIMPLE_OPERATOR op_type);

/* This takes a string exp representing an arithmetic expression involving +, -, *, /, %, (, ), integers, and variable names, and
    it evaluates the expression, returning the result on success and throwing an error with simple_error on failure. */
int exp_evaluate(simple_line_t * line, char * expression, simple_variable_t variable_map[26]);

#endif
