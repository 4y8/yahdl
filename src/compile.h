#ifndef __COMPILE_H_
#define __COMPILE_H_

#include "gram.h"

struct ir {
	enum {
		IR_STACK, IR_GLOBAL, IR_OP, IR_GATE
	}          type;
	int        n;
	struct ir *args;
};

struct decl_ir {
	int        size;
	struct ir *body;
};

struct env {
	int    len;
	char **elems;
};

short *compile(int, struct decl *);

#endif // __LEXER_H
