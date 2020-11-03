#ifndef __LEXER_H_
#define __LEXER_H_

struct token {
	enum {
		T_END, T_IS, T_CHIP, T_IN, T_OUT,     /* Keywords */
		T_IDE,
		T_LPAR, TRPAR, T_COMA, T_EQU, T_SEMI, /* Symbols */
		T_EOF
	}     type;
	char *ide;
};

#endif // __LEXER_H
