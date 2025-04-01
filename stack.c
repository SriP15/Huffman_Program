#include "node.h"
#include "stack.h"
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//The struct for a Stack.
//Contains a uint32_t top value, and uint32_t capacity value.
//Also contains an array of node pointers called items.
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

//Dynamically allocates memory for a Stack, and initializes it.
//Returns a stack pointer.
//
//capacity: a uint32_t value that stores the capacity of the stack.
Stack *stack_create(uint32_t capacity) {
    Stack *stack = (Stack *) malloc(capacity * sizeof(Stack));
    if (stack) {
        stack->top = 0;
        stack->capacity = capacity;
        Node **items_list = (Node **) calloc(capacity, sizeof(Node *));
        /*for (uint32_t i = 0; i < capacity; i += 1) {
            items_list[i] = (Node *) calloc(1, sizeof(Node));
        }*/
        stack->items = items_list;
        return stack;
    } else {
        free(stack);
        return ((Stack *) NULL);
    }
}

//Frees memory used by a Stack.
//Returns nothing (void).
//
//s: a pointer to a pointer of a stack.
void stack_delete(Stack **s) {
    if (*s) {
        /*for (uint32_t i = 0; i < (*s)->capacity; i += 1) {
            node_delete(&((*s)->items[i]));
	    //free((*s)->items[i]);
            (*s)->items[i] = NULL;
        }*/
        free((*s)->items);
        (*s)->items = NULL;
        free(*s);
        (*s) = NULL;
    }
}

//Checks to see if a Stack is empty.
//Returns a boolean. If the stack is empty: returns true. If the
//stack is not empty returns false.
//
//s: a pointer to a stack.
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

//Checks to see if a Stack is full.
//Returns a boolean. If the stack is full: returns true. If the
//stack is not full returns false.
//
//s: a pointer to a stack.
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

//Finds the size of a stack.
//Returns a uint32_t that indicates the size of a stack.
//
//s: a pointer to a stack.
uint32_t stack_size(Stack *s) {
    return (s->top);
}

//Pushes a node onto a stack.
//Returns a bool that indicates if the node was pushed successfully.
//Returns true if node was pushed. Returns false if stack is full, and
//so the node was not pushed.
//
//s: a pointer to a stack.
//n: a pointer to a node that is to be pushed onto the stack.
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top += 1;
    return true;
}

//Pops a node from a stack, saving the value to *n.
//Returns a bool that indicates if the node was popped successfully.
//Returns true if node was popped. Returns false if stack is empty, and
//so the node was not popped.
//
//s: a pointer to a stack.
//n: a pointer to a pointer of node that is to be pooped from the stack.
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top -= 1;
    *n = s->items[s->top];
    return true;
}

//A debugger function that prints the values of a stack. The stack must already be created.
//Returns void.
//
//s: a pointer to a stack.
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < (s)->capacity; i += 1) {
        printf("stack[%" PRIu32 "] value is %" PRIu8 ",frequency is %" PRIu64 ".\n", i,
            s->items[i]->symbol, s->items[i]->frequency);
    }
}
