#ifndef _TCLOGO_NODE_H_
#define _TCLOGO_NODE_H_

#include <tclogo/svg.h>

struct logo;

enum node_type {
    NODE_FORWARD,
    NODE_LEFT,
    NODE_RIGHT,
    NODE_REPEAT,
    NODE_COLOR,
    NODE_MOVE,
    NODE_SET_ANGLE,
    NODE_GROUP_BEGIN,
    NODE_GROUP_END,
    NODE_USE,
    NODE_MOVE_TO,
    NODE_RECTANGLE
};

enum arg_type {
    ARG_INT,
    ARG_STR,
    ARG_NODE
};

struct arg {
    enum arg_type type;

    union {
        int             val;
        char 		   *str;
        struct node    *subnode;
    };
};

struct node {
    enum node_type         type;
    struct arg 			  *args; 		/* Array of args */
    unsigned int           arg_count;
    struct node 		  *next;
    void (*execute) (struct logo*, const struct node*);
};

struct node    *node_new		(enum node_type type,
								 unsigned int arg_count);

void 			node_free		(struct node *p);

void 			node_append		(struct node *node,
								 struct node *new);

void 			node_print		(struct node *p);

int 			node_get_int	(const struct node *node,
								 unsigned int i);

char 		   *node_get_str	(const struct node *node,
								 unsigned int i);

struct node    *node_get_subnode(const struct node *node,
								 unsigned int i);

void 			node_set_int	(struct node *node,
								 unsigned int i,
								 int val);

void 			node_set_str	(struct node *node,
								 unsigned int i,
								 const char *str);

void 			node_set_subnode(struct node *node,
								 unsigned int i,
								 struct node *subnode);

#endif
