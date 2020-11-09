#include <stdlib.h>

#include "vm.h"

struct stack stack         = {.p = 0};
struct stack address_stack = {.p = 0};

static void push(short, struct stack *);
static short pop(struct stack *);

short vm(int, short[]);

static void
push(short s, struct stack *stack)
{
	stack->stack[MAX_STACK_SIZE - (stack->p++)] = s;
}

static short
pop(struct stack *stack)
{
	return stack->stack[MAX_STACK_SIZE - (--stack->p)];
}

short
vm(int p, short mem[])
{
	while (p)
		switch (mem[p] & 0xF) {
		case OP_RET:
			p = pop(&address_stack);
			break;
		case OP_NAND:
			push(!(pop(&stack) & pop(&stack)), &stack);
			break;
		case OP_PUSH:
			push(mem[p] >> 4, &stack);
			break;
		case OP_LOAD:
			push(mem[MAX_STACK_SIZE - stack.p + (mem[p] >> 4)],
			     &stack);
			break;
		case OP_CALL:
			push(p, &address_stack);
			p = mem[p] >> 4;
			break;
		}
	return pop(&stack);
}
