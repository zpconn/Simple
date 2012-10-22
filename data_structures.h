/*
 * Simple Interpreter
 * Group 3
 * CS 2110
 */

#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

/* I have a cool idea. I call it the "wheel." I think it may catch on. */

/* This queue is specifically for the expression parsing and so has some data specific to that application. */

enum QUEUE_DATA_TYPE
{
	NUMBER, OPERATOR
};

typedef struct stack
{
	int data;
	struct stack * next;	
} stack_struct_t;

typedef struct queue
{
	int data;
	enum QUEUE_DATA_TYPE type;
	struct queue * next;
} queue_t;

/* Pushes data onto the top of the provided stack. */
void stack_push(stack_struct_t ** top, int data);

/* Pops the top-most item off the stack and returns it. */
int stack_pop(stack_struct_t ** top);

/* Returns 1 if top is the empty stack, 0 otherwise. */
int stack_empty(stack_struct_t * top);

/* Analogous functions for the queue structure: */

void enqueue(queue_t ** head, queue_t ** tail, int data, enum QUEUE_DATA_TYPE type);

int dequeue(queue_t ** head, queue_t ** tail);

int queue_empty(queue_t * head);

#endif
