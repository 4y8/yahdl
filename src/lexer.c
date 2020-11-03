/* Example:
 * chip and3 is
 *   in a, b, c;
 *   out o;
 *   o1 := and(a, b);
 *   o  := and(o1, c);
 * end and3
 */

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

static struct token mktoken(unsigned int);
static struct token mktide(char *);


static struct token
mktoken(unsigned int type)
{
	return (struct token){.type = type};
}

static struct token
mktide(char *ide)
{
	return (struct token){.type = T_IDE, .ide = ide};
}
