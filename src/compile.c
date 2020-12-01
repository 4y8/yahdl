#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "compile.h"
#include "vm.h"

static void append_env(struct env *, char *);
static int find_env(struct env, char *);
static void push_env(struct env *);
static void pop_env(struct env *);

static int builtin(char *);
static struct ir node_to_ir(struct node, struct env);
static struct decl_ir *decls_to_ir(int, struct decl *);

static void compile_ir(struct ir, int *, short *);
static void compile_decl_ir(struct decl_ir, int *, short *);
static void compile_decls_ir(int, struct decl_ir *, int *, short *);

void compile(int, struct decl *, int *, short *);

static void
append_env(struct env *env, char *e)
{
	++env->len;
	env->elems = realloc(env->elems, env->len * sizeof(char *));
	*(env->elems + env->len - 1) = e;
}

static int
find_env(struct env env, char *s)
{
	for (int i = 0; i < env.len; ++i)
		if (!strcmp(s, env.elems[i]))
			return i;

	return -1;
}

static void
push_env(struct env *env)
{
	append_env(env, "");
}

static void
pop_env(struct env *env)
{
	--env->len;
}

static int
builtin(char *s)
{
	switch (*s) {
	case 'n':
		if (!strcmp(s + 1, "and"))
			return OP_NAND;

		break;
	}
	return -1;
}

static struct ir
node_to_ir(struct node n, struct env env)
{
	struct ir i;

	switch (n.type) {
	case N_IDE: {
		int t = builtin(n.name);
		if (t < 0)
			t = find_env(env, n.name);
		i = (struct ir){.type = IR_STACK, .
			        n     = env.len - t};
		break;
	}

	case N_GATE: {
		struct ir e;
		e.args = malloc(n.narg * sizeof(struct ir));
		e.type = IR_GATE;
		e.narg = n.narg;
		e.n    = builtin(n.name);
		for (int i = 0; i < n.narg; ++i) {
			e.args[i] = node_to_ir(n.args[i], env);
			push_env(&env);
		}
		/* May be unuseful */
		for (int i = 0; i < n.narg; ++i)
			pop_env(&env);
		i = e;
		break;
	}
	}
	return i;
}

static struct decl_ir *
decls_to_ir(int len, struct decl *decls)
{
	struct decl_ir *ir_decls;
	struct decl     decl;

	ir_decls = malloc(len * sizeof(struct decl_ir));
	for (int i = 0; i < len; ++i) {
		decl        = decls[i];
		ir_decls[i] = (struct decl_ir){
			.body = malloc(decl.size + 1),
			.size = decl.size + 1
		};
		struct env env = {.len   = 0,
			          .elems = malloc(sizeof(char *))};
		for (int j = 0; j < decls[i].narg; ++j)
			append_env(&env, decl.args[j]);
		for (int j = 0; j < decl.size; ++j) {
			ir_decls[i].body[j] = node_to_ir(decl.body[j].node,
			                                 env);
			append_env(&env, decl.body[j].node.name);
		}
		ir_decls[i].body[decl.size] = node_to_ir(decl.out, env);
	}
	return ir_decls;
}

static void
compile_ir(struct ir n, int *len, short *p)
{
	switch (n.type) {
	case IR_OP:
		p[*len] = n.n;
		++(*len);
		break;
	case IR_STACK:
		p[*len] = (n.n << 4) | OP_LOAD;
		++(*len);
		break;
	case IR_GATE:
		for (int i = 0; i < n.narg; ++i)
			compile_ir(n.args[i], len, p);

		//p[*len] = OP_CALL | n.n;
		p[*len] = n.n;
		++(*len);
		p[*len] = OP_RES;
		++(*len);

		break;
	default:
		break;
	}
}

static void
compile_decl_ir(struct decl_ir d, int *len, short *p)
{
	for (int i = 0; i < d.size; ++i) {
		compile_ir(d.body[i], len, p);
		p[*len] = OP_RES;
		++(*len);
	}
	p[*len] = OP_RET;
	++(*len);
}

static void
compile_decls_ir(int len, struct decl_ir *d, int *plen, short *p)
{
	for (int i = 0; i < len; ++i)
		compile_decl_ir(d[i], plen, p);
}

void
compile(int len, struct decl *d, int *plen, short *p)
{
	struct decl_ir *il;

	il = decls_to_ir(len, d);
	compile_decls_ir(len, il, plen, p);
}
