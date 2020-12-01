#include <stdlib.h>
#include <stdio.h>

#include "debug.h"

#include "lexer.h"
#include "gram.h"
#include "compile.h"
#include "vm.h"

static char *tokens[T_EOF + 1] = {
	[T_END]  = "keyword: end",  [T_IS]   = "keyword: is",
	[T_CHIP] = "keyword: chip", [T_LET]  = "keyword: let",
	[T_IDE]  = "identifier: ",
	[T_LPAR] = "symbol: (",     [T_RPAR] = "symbol: )",
	[T_COMA] = "symbol: ,",     [T_EQU]  = "symbol: =",
	[T_SEMI] = "symbol: ;",
	[T_EOF]  = "EOF"
};

static char *instructions[OP_RET + 1] = {
	[OP_CALL] = "call", [OP_PUSH] = "push", [OP_LOAD] = "load",
	[OP_NAND] = "nand", [OP_POP]  = "pop",  [OP_RES]  = "res",
	[OP_RET]  = "ret"
};

static void prop(char *);

void show_string(char **);
void show_token(struct token *);
void show_node(struct node *);
void show_decl(struct decl *);
void show_assign(struct assign *);
void show_ir(struct ir *);
void show_decl_ir(struct decl_ir *);
void show_asm(short *);

void show_list(void (*)(void *), int, void *, size_t);

static void
prop(char *p)
{
	printf("\"%s\":", p);
}

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
	printf("{");
	switch (n->type) {
	case N_IDE:
		prop("identifier");
		show_string(&n->name);
		break;
	case N_GATE:
		prop("gate");
		printf("{");
		prop("name");
		show_string(&n->name);
		printf(",");
		prop("args");
		show_list((void (*)(void *))show_node, n->narg, n->args,
		          sizeof(struct node));
		printf("}");
		break;
	}
	printf("}");
}

void
show_decl(struct decl *d)
{
	printf("{\"declaration\": {\"name\":\"%s\",", d->name);
	prop("args");
	show_list((void (*)(void *))show_string, d->narg, d->args,
	          sizeof(char *));
	printf(",");
	prop("body");
	show_list((void (*)(void *))show_assign, d->size, d->body,
	          sizeof(struct assign));
	printf(",");
	prop("return");
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
	printf("{");
	switch (i->type) {
	case IR_STACK:
		prop("stack");
		printf("%d", i->n);
		break;
	case IR_GATE:
		prop("gate");
		printf("{");
		prop("index");
		printf("%d", i->n);
		printf(",");
		prop("args");
		show_list((void (*)(void *))show_ir, i->narg, i->args,
		          sizeof(struct ir));
		printf("}");
		break;
	case IR_OP:
		break;
	case IR_GLOBAL:
		break;
	}
	printf("}");
}

void
show_decl_ir(struct decl_ir *d)
{
	printf("{");
	prop("declaration_ir");
	printf("{");
	prop("body");
	show_list((void (*)(void *))show_ir, d->size, d->body,
	          sizeof(struct ir));
	printf("}");
	printf("}");
}

void
show_asm(short *s)
{
	printf("{");
	prop("instruction");
	show_string(&instructions[*s & 0b1111]);
	switch ((enum op_code)(*s & 0b1111)) {
	case OP_CALL:
	case OP_LOAD:
	case OP_PUSH:
		printf(",");
		prop("value");
		printf("%d", *s & 0b1111);
		break;
	default:
		break;
	}
	printf("}");
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
