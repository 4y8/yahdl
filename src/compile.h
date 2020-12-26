#ifndef __COMPILE_H_
#define __COMPILE_H_

#include "gram.h"

struct ir {
	enum {
		IR_STACK, IR_GLOBAL, IR_OP, IR_GATE
	}          type;
	int        n;
	int        narg;
	int        is_builtin;
	struct ir *args;
};

struct decl_ir {
	int        size;
	struct ir *body;
	char      *name;
};

struct env {
	int    len;
	char **elems;
};

struct global_env {
	int len;
	struct gelem {
		char *s;
		int pos;
		int size;
	} *elems;
};

void compile(int, struct decl *, int *, short *, struct global_env *);

#endif // compile.h
