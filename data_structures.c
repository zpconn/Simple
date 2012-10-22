/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#include <stdlib.h>
#include "error.h"
#include "data_structures.h"

void stack_push(stack_struct_t ** top, int data)
{
	stack_struct_t * new_node = malloc(sizeof(stack_struct_t));
	
	if (new_node)
	{
		new_node->data = data;
		new_node->next = *top;
		*top = new_node;
	}
	else
	{
		simple_error(MYSTERY, NULL, "failed to allocate memory to push a new node onto a stack");
	}
}

int stack_pop(stack_struct_t ** top)
{
	stack_struct_t * temp;
	int pop_value;
	
	temp = *top;
	pop_value = (*top)->data;
	*top = (*top)->next;
	free(temp);
	
	return pop_value;
}

int stack_empty(stack_struct_t * top)
{
	return top == NULL;
}

void enqueue(queue_t ** head, queue_t ** tail, int data, enum QUEUE_DATA_TYPE type)
{
	queue_t * new_node = malloc(sizeof(queue_t));
	
	if (new_node)
	{
		new_node->data = data;
		new_node->type = type;
		new_node->next = NULL;
		
		if (queue_empty(*head))
		{
			*head = new_node;
		}
		else
		{
			(*tail)->next = new_node;
		}
		
		*tail = new_node;
	}
	else
	{
		simple_error(MYSTERY, NULL, "failed to allocate memory to enqueue a new node onto a queue");
	}
}

int dequeue(queue_t ** head, queue_t ** tail)
{
	int value;
	queue_t * temp;
	
	value = (*head)->data;
	temp = *head;
	*head = (*head)->next;
	
	if (*head == NULL)
	{
		*tail = NULL;
	}
	
	free(temp);
	
	return value;
}

int queue_empty(queue_t * head)
{
	return head == NULL;
}
