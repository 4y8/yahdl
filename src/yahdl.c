#include <stdlib.h>
#include <stdio.h>

#include "vm.h"
#include "gram.h"
#include "lexer.h"
#include "compile.h"
#include "debug.h"

int
main()
{
	struct decl *d;
	int          len = 0, plen = 0;
	short *      p;

	p = malloc(1024 * sizeof(short));
	init_lexer("tests/not.yahdl");
	d = program(&len);
	show_decl(d);
	compile(len, d, &plen, p);
	show_list((void (*)(void *))show_asm, plen, p, sizeof(short));
}
