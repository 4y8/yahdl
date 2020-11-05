#include <stdlib.h>

#include "gram.h"

static struct node mknide(char *);
static struct node mkngate(char *, int, struct node *);
static struct decl mkdecl(char *, int, struct node *);

static struct node get_node(void);

struct decl *program(void);


static struct node mknide(char *name)
{
	return (struct node){.type = N_IDE, .name = name};
}
