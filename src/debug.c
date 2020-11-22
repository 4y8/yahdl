#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "gram.h"
#include "compile.h"

void show_token(struct token *);
void show_node(struct node *);
void show_decl(struct decl *);
void show_ir(struct ir *);
void show_decl_ir(struct decl_ir *);
void show_asm(short *);

void show_list(void (*)(void *), int, void *);

void
show_token(struct token *t)
{
}

void
show_node(struct node *n)
{
}

void
show_decl(struct decl *d)
{
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
show_list(void (*f)(void *), int n, void *l)
{
	for (int i = 0; i < n; ++i)
		f(l + i);
}
