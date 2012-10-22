/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "expression.h"
#include "helper.h"
#include "error.h"

char * pad_string(char * str)
{
	int size = strlen(str);
	int num_chars = count_occurrences(str, '(', size) + count_occurrences(str, ')', size)
	              + count_occurrences(str, '+', size) + count_occurrences(str, '-', size)
	              + count_occurrences(str, '*', size) + count_occurrences(str, '/', size)
				  + count_occurrences(str, '%', size);
	int new_size = size + num_chars * 2;
	char * new_str = malloc(new_size + 1);
	
	long i = 0, j = 0;
	for (; j < size && i < new_size; ++i, ++j)
	{
		if (str[j] == '(' || str[j] == ')' || str[j] == '+' || str[j] == '-' ||
		    str[j] == '*' || str[j] == '/' || str[j] == '%')
		{
			new_str[i++] = ' ';
			new_str[i++] = str[j];
			new_str[i] = ' ';
		}
		else
		{
			new_str[i] = str[j];
		}
	}
	
	new_str[i] = '\0';
	
	return new_str;
}

enum SIMPLE_OPERATOR token_determine_op(char * token)
{
	if (strcmp(token, "(") == 0)
	{
		return LEFT_PARENTHESIS;
	}
	else if (strcmp(token, ")") == 0)
	{
		return RIGHT_PARENTHESIS;
	}
	else if (strcmp(token, "+") == 0)
	{
		return PLUS;
	}
	else if (strcmp(token, "-") == 0)
	{
		return MINUS;
	}
	else if (strcmp(token, "*") == 0)
	{
		return TIMES;
	}
	else if (strcmp(token, "/") == 0)
	{
		return DIVIDE;
	}
	else if (strcmp(token, "%") == 0)
	{
		return MODULUS;
	}
	else
	{
		return OP_NO_MATCH;
	}
}

int evaluate_operator(enum SIMPLE_OPERATOR op, int operand1, int operand2)
{
	if (op == PLUS)
	{
		return operand1 + operand2;
	}
	else if (op == MINUS)
	{
		return operand1 - operand2;
	}
	else if (op == TIMES)
	{
		return operand1 * operand2;
	}
	else if (op == DIVIDE)
	{
		return operand1 / operand2;
	}
	else if (op == MODULUS)
	{
		return operand1 % operand2;
	}
	
	return 0;
}

int op_precedence(enum SIMPLE_OPERATOR op_type)
{
	/* This is fairly simple in our case. */
	if (op_type == TIMES || op_type == DIVIDE || op_type == MODULUS)
	{
		return 10;
	}
	else
	{
		return 0;
	}
}

int exp_evaluate(simple_line_t * line, char * expression, simple_variable_t variable_map[26])
{
	/* STEP 1: convert the expression from infix to postfix. */
	
	/* We will use two structures, a queue to store the output and a stack to store operators during the conversion. */
	queue_t * output_head = NULL, * output_tail = NULL;
	stack_struct_t * operators = NULL;
	
	/* For step 2, we will need a stack to facilitate the evaluation of the postfix expression. */
	stack_struct_t * eval_stack = NULL;
	
	/* We will traverse the expression with this pointer. */
	char * token;
	
	/* We will use strtok, which modifies the line, so we copy the line first. */
	char * copy = pad_string(expression);
	
	token = strtok(copy, " ");
	
	for (; token != NULL; token = strtok(NULL, " "))
	{		
		/* We need to figure out what this token represents. */
		
		if (is_integer(token))
		{
			enqueue(&output_head, &output_tail, atoi(token), NUMBER);
		}
		else if (token_determine_op(token) != OP_NO_MATCH)
		{
			enum SIMPLE_OPERATOR op_type = token_determine_op(token);
			
			if (op_type == LEFT_PARENTHESIS)
			{
				stack_push(&operators, LEFT_PARENTHESIS);
			}
			else if (op_type == RIGHT_PARENTHESIS)
			{
				if (stack_empty(operators))
				{
					simple_error(SYNTAX_ERROR, line, "incorrect matching of parentheses in arithmetic expression; inappropriate right parenthesis");
				}
				
				while (operators->data != LEFT_PARENTHESIS)
				{
					if (stack_empty(operators))
					{
						simple_error(SYNTAX_ERROR, line, "incorrect matching of parentheses in arithmetic expression");
					}
					
					enqueue(&output_head, &output_tail, stack_pop(&operators), OPERATOR);
					
					if (stack_empty(operators))
					{
						simple_error(SYNTAX_ERROR, line, "incorrect matching of parentheses in arithmetic expression");
					}
				}
				
				stack_pop(&operators);
			}
			else
			{
				while (!stack_empty(operators) && op_precedence(op_type) <= op_precedence((enum SIMPLE_OPERATOR)operators->data)
				       && operators->data != LEFT_PARENTHESIS && operators->data != RIGHT_PARENTHESIS)
				{
					enqueue(&output_head, &output_tail, stack_pop(&operators), OPERATOR);
				}
				
				stack_push(&operators, op_type);
			}
		}
		else if (check_variable_name_no_error(token))
		{
			enqueue(&output_head, &output_tail, simple_lookup_variable(variable_map, token, line), NUMBER);
		}
		else
		{
			simple_error(SYNTAX_ERROR, line, "invalid or unrecognizable symbol is in the arithmetic expression");
		}
	}
	
	while (!stack_empty(operators))
	{
		if (operators->data == LEFT_PARENTHESIS || operators->data == RIGHT_PARENTHESIS)
		{
			simple_error(SYNTAX_ERROR, line, "incorrect matching of parentheses in arithmetic expression");
		}
		
		enqueue(&output_head, &output_tail, stack_pop(&operators), OPERATOR);
	}
	
	/* STEP 2: evaluate the postfix expression represented by the queue output. */
	
	/* We go "left to right" in the output queue. We add numbers to eval_stack, and when we reach an operator, we evaluate the
	   operator and the last two items in the eval_stack (by popping them) and then we push the result onto eval_stack. If the
	   expression was well-formed, this will always be possible and one number will result at the end, the value of the
	   original expression. */
	
	while (!queue_empty(output_head))
	{
		int operand1 = 0, operand2 = 0;
		enum QUEUE_DATA_TYPE type = output_head->type;
		int value = dequeue(&output_head, &output_tail);
		
		if (type == OPERATOR)
		{
			if (stack_empty(eval_stack))
			{
				simple_error(SYNTAX_ERROR, line, "incorrect syntax for arithmetic expression");
			}
			
			operand2 = stack_pop(&eval_stack);
			
			if (stack_empty(eval_stack))
			{
				simple_error(SYNTAX_ERROR, line, "incorrect syntax for arithmetic expression");
			}
			
			operand1 = stack_pop(&eval_stack);
			
			stack_push(&eval_stack, evaluate_operator((enum SIMPLE_OPERATOR)value, operand1, operand2));
		}
		else if (type == NUMBER)
		{
			stack_push(&eval_stack, value);
		}
	}
	
	if (stack_empty(eval_stack) || eval_stack->next != NULL)
	{
		simple_error(SYNTAX_ERROR, line, "incorrect syntax for arithmetic expression");
	}
	
	free(copy);
	
	return eval_stack->data;
}
