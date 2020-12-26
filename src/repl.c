#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <editline.h>

#include "repl.h"

#include "compile.h"
#include "vm.h"

static char *prompt = "> ";

void
repl(void)
{
	struct env *env;
	char *p;

	while ((p = readline(prompt)) != NULL) {
		puts(p);
		if (!memcmp(p, "load", 4)) {

		}
		free(p);
	}
}
