#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "compile.h"
#include "vm.h"

static void append_env(struct env *, char *);
static int find_env(struct env, char *);
static void push_env(struct env *);
static void pop_env(struct env *);

static void append_genv(struct global_env *, char *, int);
static int find_genv(struct global_env, char *);

static int builtin(char *);
static struct ir node_to_ir(struct node, struct env *, struct global_env *);
static struct decl_ir *decls_to_ir(int, struct decl *, struct global_env *);

static void compile_ir(struct ir, int *, short *);
static void compile_decl_ir(struct decl_ir, int *, short *);
static void compile_decls_ir(int, struct decl_ir *, int *, short *);

void compile(int, struct decl *, int *, short *, struct global_env *);

static void
append_env(struct env *env, char *e)
{
	env->elems = realloc(env->elems, ++env->len * sizeof(char *));
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

static void
append_genv(struct global_env *genv, char *s, int size)
{
	genv->elems = realloc(genv->elems, ++genv->len * sizeof(struct gelem));
	if (genv->len > 1) {
		int pos = genv->elems[genv->len - 2].pos +
			genv->elems[genv->len - 2].size;
		genv->elems[genv->len - 1] = (struct gelem){
			.s = s,
			.pos = pos,
			.size = size
		};
	} else
		genv->elems[0] = (struct gelem){.s = s, .pos = 0, .size = size};
}

static int
find_genv(struct global_env genv, char *s)
{
	for (int i = 0; i < genv.len; ++i)
		if (!strcmp(s, genv.elems[i].s))
			return i;

	return -1;
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
node_to_ir(struct node n, struct env *env, struct global_env *genv)
{
	struct ir i;

	switch (n.type) {
	case N_IDE: {
		int t = find_env(*env, n.name);
		i = (struct ir){.type = IR_STACK, .
			        n     = env->len - t - 1};
		push_env(env);
		break;
	}

	case N_GATE:
		i.args = malloc(n.narg * sizeof(struct ir));
		i.type = IR_GATE;
		i.narg = n.narg;
		i.n    = builtin(n.name);
		if (i.n < 0) {
			i.n = genv->elems[find_genv(*genv, n.name)].pos;
			i.is_builtin = 0;
		} else
			i.is_builtin = 1;
		for (int j = 0; j < n.narg; ++j) {
			i.args[j] = node_to_ir(n.args[j], env, genv);
		}
		for (int j = 0; j < n.narg; ++j)
			pop_env(env);
		break;
	}
	return i;
}

static struct decl_ir *
decls_to_ir(int len, struct decl *decls, struct global_env *genv)
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
			                                 &env, genv);
			append_env(&env, decl.body[j].node.name);
		}
		ir_decls[i].body[decl.size] = node_to_ir(decl.out, &env, genv);
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

		if (n.is_builtin)
			p[*len] = n.n;
		else
			p[*len] = OP_CALL | n.n;
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
compile(int len, struct decl *d, int *plen, short *p, struct global_env *genv)
{
	struct decl_ir *il;

	il = decls_to_ir(len, d, genv);
	compile_decls_ir(len, il, plen, p);
}
