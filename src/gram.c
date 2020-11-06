#include <stdlib.h>

#include "gram.h"
#include "lexer.h"

static struct node mknide(char *);
static struct node mkngate(char *, int, struct node *);
static struct decl mkdecl(char *, int, struct node *);

static void assert(int);
static int peek(int);
static struct node get_node(void);

struct decl *program(void);

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
mkdecl(char *name, int size, struct node *body)
{
	return (struct decl){.name = name,
		             .body = body,
		             .size = size};
}

static void
assert(int type)
{
	if (type != next_token().type)
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
			}
			return mkngate(name, narg, args);
		} else {
			putback_token(t);
			return mknide(name);
		}
		break;
	}
	default: exit(1);
	}
}
