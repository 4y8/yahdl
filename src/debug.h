#ifndef __DEBUG_H_
#define __DEBUG_H_

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

#endif // debug.h
