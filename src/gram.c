#include <stdlib.h>

#include "gram.h"
#include "lexer.h"

static struct node mknide(char *);
static struct node mkngate(char *, int, struct node *);
static struct decl mkdecl(char *, int, struct assign *, int, char **, struct node);
static struct assign mkassign(char *, struct node);

static void assert(int);
static int peek(int);
static char *identifier(void);
static struct node get_node(void);
static struct decl chip_decl(void);

struct decl *program(int *);

static struct node
mknide(char *name)
{
	return (struct node){.type = N_IDE, .name = name};
}

static struct node
mkngate(char *name, int narg, struct node *args)
{
	return (struct node){.type = N_GATE,
		             .name = name,
		             .args = args,
		             .narg = narg};
}

static struct decl
mkdecl(char *name, int size, struct assign *body, int narg, char **args,
       struct node out)
{
	return (struct decl){.name = name,
		             .size = size,
		             .body = body,
		             .narg = narg,
		             .args = args,
		             .out  = out};
}

static struct assign
mkassign(char *name, struct node node)
{
	return (struct assign){.name = name, .node = node};
}

static void
assert(int type)
{
	if (type != next_token().type)
		exit(1);
}

static char *
identifier(void)
{
	struct token t;

	t = next_token();
	if (t.type == T_IDE)
		return t.ide;
	else
		exit(1);
}

static int
peek(int type)
{
	struct token t;

	t = next_token();
	if (t.type == type)
		return 1;
	else {
		putback_token(t);
		return 0;
	}
}

static struct node
get_node(void)
{
	struct token t;

	t = next_token();
	switch (t.type) {
	case T_IDE: {
		char *name = t.ide;
		t = next_token();
		if (t.type == T_LPAR) {
			struct node *args;
			int          narg = 0;
			args = malloc(sizeof(struct node));
			while (!peek(T_RPAR)) {
				++narg;
				args = realloc(args, narg * sizeof(struct node));
				*(args + narg - 1) = get_node();
				assert(T_COMA);
			}
			return mkngate(name, narg, args);
		} else {
			putback_token(t);
			return mknide(name);
		}
	}
	default: exit(1);
	}
}

static struct decl
chip_decl(void)
{
	int            narg, size;
	char **        args, *name;
	struct node    out;
	struct assign *body;

	size = 0;
	narg = 0;
	assert(T_CHIP);
	name = identifier();
	assert(T_LPAR);
	args = malloc(sizeof(char *));
	while (!peek(T_RPAR)) {
		++narg;
		args = realloc(args, narg * sizeof(char *));
		*(args + narg - 1) = identifier();
		assert(T_COMA);
	}
	assert(T_IS);
	body = malloc(sizeof(struct assign));
	while (peek(T_LET)) {
		char *      name;
		struct node node;
		++size;
		body = realloc(body, size * sizeof(struct assign));
		name = identifier();
		assert(T_EQU);
		node = get_node();
		*(body + size - 1) = mkassign(name, node);
		assert(T_SEMI);
	}
	out = get_node();
	assert(T_SEMI);
	assert(T_END);
	assert(T_CHIP);
	return mkdecl(name, size, body, narg, args, out);
}

struct decl *
program(int *size)
{
	static struct decl *program;

	*size   = 0;
	program = malloc(sizeof(struct decl));
	while (!peek(T_EOF)) {
		++(*size);
		program = realloc(program, *size * sizeof(struct decl));
		*(program + *size - 1) = chip_decl();
	}
	return program;
}
