#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "gram.h"
#include "compile.h"

static char *tokens[T_EOF + 1] = {
	[T_END]  = "keyword: end",  [T_IS]   = "keyword: is",
	[T_CHIP] = "keyword: chip", [T_LET]  = "keyword: let",
	[T_IDE]  = "identifier: ",
	[T_LPAR] = "symbol: (",     [T_RPAR] = "symbol: )",
	[T_COMA] = "symbol: ,",     [T_EQU]  = "symbol: =",
	[T_SEMI] = "symbol: ;",
	[T_EOF]  = "EOF"
};

void show_string(char **);
void show_token(struct token *);
void show_node(struct node *);
void show_decl(struct decl *);
void show_assign(struct assign *);
void show_ir(struct ir *);
void show_decl_ir(struct decl_ir *);
void show_asm(short *);

void show_list(void (*)(void *), int, void *, size_t);

void
show_string(char **s)
{
	printf("\"%s\"", *s);
}

void
show_token(struct token *t)
{
	printf("%s", tokens[t->type]);
	if (t->type == T_IDE) printf("%s", t->ide);
}

void
show_node(struct node *n)
{
	switch (n->type) {
	case N_IDE:
		printf("{\"identifier\":\"%s\"}", n->name);
		break;
	case N_GATE:
		printf("{\"gate\":{\"name\":\"%s\",", n->name);
		printf("\"args\":");
		show_list((void (*)(void *))show_node, n->narg, n->args,
		          sizeof(struct node));
		printf("}}");
		break;
	}
}

void
show_decl(struct decl *d)
{
	printf("{\"declaration\": {\"name\":\"%s\",", d->name);
	printf("\"args\":");
	show_list((void (*)(void *))show_string, d->narg, d->args,
	          sizeof(char *));
	printf(", \"body\":");
	show_list((void (*)(void *))show_assign, d->size, d->body,
	          sizeof(struct assign));
	printf(",\"return\":");
	show_node(&d->out);
	printf("}}");
}

void
show_assign(struct assign *a)
{
	printf("variable: %s, equal: ", a->name);
	show_node(&a->node);
}

void
show_ir(struct ir *i)
{
}

void
show_decl_ir(struct decl_ir *d)
{
}

void
show_asm(short *s)
{
}

void
show_list(void (*f)(void *), int n, void *l, size_t size)
{
	printf("[");
	for (int i = 0; i < n; ++i) {
		f(l + i * size);
		if (i != n - 1) printf(", ");
	}
	printf("]");
}
