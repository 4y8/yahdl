#ifndef __GRAM_H_
#define __GRAM_H_

struct node {
	enum {
		N_IDE, N_GATE
	}            type;
	char *       name;
	struct node *args;
	int          narg;
};

struct assign {
	char *      name;
	struct node node;
};

struct decl {
	char *         name;
	int            size;
	struct assign *body;
	int            narg;
	char **        args;
	struct node    out;
};

struct decl *program(void);

#endif // __GRAM_H
