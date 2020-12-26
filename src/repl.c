#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <editline.h>

#include "repl.h"

#include "compile.h"
#include "lexer.h"
#include "vm.h"

#define BUFFER_SIZE 4096

static char *prompt = "> ";

void
repl(void)
{
	struct global_env genv;
	char *p;
	short buffer[BUFFER_SIZE];
	int len, plen;
	struct decl *d;

	plen = 0;
	genv = (struct global_env){
		.len = 0,
		.elems = malloc(sizeof(struct gelem))
	};
	while ((p = readline(prompt)) != NULL) {
		if (!memcmp(p, "load", 4)) {
			p += 4;
			while (*(++p) == ' ' || *p == '\t');
			init_lexer(p);
			d = program(&len);
			compile(len, d, &plen, buffer, &genv);
			close_lexer();
		} else {
			int save_plen = plen;
		}
	}
}
