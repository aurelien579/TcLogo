#ifndef NODE_H_
#define NODE_H_

#define NODE_FORWARD        1
#define NODE_LEFT           2
#define NODE_RIGHT          3
#define NODE_REPEAT         4
#define NODE_COLOR          5
#define NODE_CREATE_CANVAS  6
#define NODE_SET_CANVAS     7
#define NODE_DRAW_CANVAS    8

#define STR_LENGTH  10

struct state;

struct node {
	int            type;
	int            int_value;
	char           str_value[STR_LENGTH];
	struct node *  subnode;
	struct node *  next;
    void (*execute) (struct state*, const struct node*);
};

struct node *node_new(int type, int int_value, char *str_value, struct node *subnode,
    void (*execute) (struct state*, const struct node*)
);
void node_free(struct node *p);
void node_append(struct node *node, struct node *new);
void node_print(struct node *p);

#endif
