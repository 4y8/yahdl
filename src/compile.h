#ifndef __COMPILE_H_
#define __COMPILE_H_

#include "gram.h"

struct ir {
	enum {
		IR_STACK, IR_GLOBAL, IR_GATE
	}          type;
	int        n;
	struct ir *args;
};

short *compile(int, struct decl *);

#endif // __LEXER_H
