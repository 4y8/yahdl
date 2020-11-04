/* Example:
 * chip and3 is
 *   in a, b, c;
 *   out o;
 *   o1 := and(a, b);
 *   o  := and(o1, c);
 * end and3
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

FILE *       in_file;
struct token unused_token;

static struct token mktoken(unsigned int);
static struct token mktide(char *);
static struct token get_token(void);
static char *lexf(int (*f)(int));
static int keyword(char *);

struct token next_token(void);
void putback_token(struct token);
void init_lexer(char *);

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

static char *
lexf(int (*f)(int))
{
	int   size, i, c;
	char *s, *p;

	i    = 0;
	size = 256;
	p    = (s = malloc(size)) - 1;

	if (!s)
		exit(1);

	while (f(c = getc(in_file)) && c) {
		/* Allocate more memory for the string if we've reached its limit. */
		if (++i >= size) {
			size += 256;
			s     = realloc(s, size);
			p     = s + i - 2;
			if (!s)
				exit(1);
		}
		*(++p) = c;
	}
	/* NUL-terminate the string. */
	s[i] = 0;
	ungetc(c, in_file);
	return s;
}

static int
keyword(char *s)
{
	switch (*s) {
	case 'c':
		if (!strcmp(s + 1, "hip"))
			return T_CHIP;

		break;
	case 'e':
		if (!strcmp(s + 1, "nd"))
			return T_END;

		break;
	case 'i':
		if (!strcmp(s + 1, "s"))
			return T_IS;
		else if (!strcmp(s + 1, "n"))
			return T_IN;

		break;

	case 'o':
		if (!strcmp(s + 1, "ut"))
			return T_OUT;

		break;
	}
	return -1;
}

static struct token
get_token(void)
{
	int c;

	while ((c = getc(in_file)) == ' ' || c == '\t');
	if (c == EOF)
		return mktoken(T_EOF);
	else if (isalpha(c)) {
		ungetc(c, in_file);
		char *ide;
		char *s = (ide = malloc(256)) - 1;
		int   type;
		while (isalpha(c = getc(in_file)))
			*(++s) = c;
		*(s + 1) = '\0';
		type     = keyword(ide);
		if (type >= 0)
			return mktoken(type);

		return mktide(ide);
	} else
		switch (c) {
		case ',': return mktoken(T_COMA);

		case ' ':
		case '\t':
		case '\n': return get_token();

		case ';': return mktoken(T_SEMI);

		case ':':
			if (getc(in_file) == '=')
				return mktoken(T_EQU);

		default:
			exit(1);
		}
}
