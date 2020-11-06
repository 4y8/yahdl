#include <stdlib.h>

#include "gram.h"
#include "lexer.h"

static struct node mknide(char *);
static struct node mkngate(char *, int, struct node *);
static struct decl mkdecl(char *, int, struct node *);

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

static struct node
get_node(void)
{
	struct token t;

	t = next_token();
	switch (t.type) {
	case T_IDE:
		break;
	default: exit(1);
	}
}
