#ifndef __VM_H_
#define __VM_H_

/*
 * Instruction format (on 16 bits):
 * yyyyyyyyyyyyxxxx where x is an opcode and y is the argument.
 * yyyyyyyyyyyyCALL jumps to the y'th word of memory
 * yyyyyyyyyyyyPUSH push y on the stack
 * yyyyyyyyyyyyLOAD pushes the y'th values on the stack
 * ____________NAND pushes the result of a nand between the two elements on the
 *                  top of the stack
 * ____________POP  pops a value from the stack
 * ____________RES  pushes the result of the last call
 * ____________RET  returns from a subroutine called with call
 */

#define MAX_STACK_SIZE    1024

enum op_code {
	OP_CALL = 0,
	OP_PUSH,
	OP_LOAD,
	OP_NAND,
	OP_POP,
	OP_RES,
	OP_RET
};

struct stack {
	short stack[MAX_STACK_SIZE];
	int   p;
};

short vm(int, short[]);

#endif // vm.h
