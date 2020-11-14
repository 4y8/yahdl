#include <stdlib.h>

#include "compile.h"

static void append_env(struct env *, char *);
static struct decl_ir *to_ir(int, struct decl *, int *);
static short *compile_ir(int, struct ir *);

short *compile(int, struct decl *);

static void
append_env(struct env *env, char *e)
{
	++env->len;
	env->elems = realloc(env->elems, env->len * sizeof(char *));
	*(env->elems + env->len - 1) = e;
}

static struct decl_ir *
to_ir(int inlen, struct decl *decls, int *outlen)
{
	for (int i = 0; i < inlen; ++i) {
		struct env env = {.len   = 0,
			          .elems = malloc(sizeof(char *))};
		for (int j = 0; j < decls[i].narg; ++j)
			append_env(&env, decls[i].args[j]);
		for (int j = 0; j < decls[i].size; ++j) {
		}
		--inlen;
	}
}
