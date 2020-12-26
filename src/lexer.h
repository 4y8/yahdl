#ifndef __LEXER_H_
#define __LEXER_H_

struct token {
	enum {
		T_END = 0, T_IS, T_CHIP, T_LET,        /* Keywords */
		T_IDE,
		T_LPAR, T_RPAR, T_COMA, T_EQU, T_SEMI, /* Symbols */
		T_EOF
	}     type;
	char *ide;
};

struct token next_token(void);
void putback_token(struct token);
void init_lexer(char *);
void close_lexer(void);

#endif // lexer.h
