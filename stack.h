#ifndef STACK_H
#define STACK_H

// A structure to represent a stack
struct Stack {
    int top;
    unsigned int capacity;
    int* array;
};
 
// function to create a stack of given capacity
struct Stack* createStack(unsigned int capacity);

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack);
 
// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack);
 
// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, int item);
 
// Function to remove an item from stack.  It decreases top by 1
int pop(struct Stack *stack);
 
// Function to return the top from stack without removing it
int peek(struct Stack *stack);

#endif // STACK_H

