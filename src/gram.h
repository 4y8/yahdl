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

struct decl {
	char *       name;
	struct node *body;
	int          size;
};

#endif // __GRAM_H
