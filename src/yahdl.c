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
	int          len = 0;

	init_lexer("tests/not.yahdl");
	d = program(&len);
	show_decl(d);
}
