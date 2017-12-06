#ifndef NODE_H_
#define NODE_H_

#define STR_LENGTH  10

#include "svg.h"

struct state;

enum node_type {
    NODE_FORWARD,
    NODE_LEFT,
    NODE_RIGHT,
    NODE_REPEAT,
    NODE_COLOR,
    NODE_MOVE
};

struct node {
	enum node_type type;
	int            int_value;
	char           str_value[STR_LENGTH];
	struct node *  subnode;
	struct node *  next;
    void (*execute) (struct state*, const struct node*);
};

struct node *node_new(enum node_type type, int int_value, char *str_value, struct node *subnode);
void node_free(struct node *p);
void node_append(struct node *node, struct node *new);
void node_print(struct node *p);

#endif
