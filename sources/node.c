#include "node.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct function {
    enum node_type node_type;
    void (*f) (struct state*, const struct node*);
};

static struct function functions[] = {
    { NODE_FORWARD, execute_forward },
    { NODE_LEFT, execute_left },
    { NODE_RIGHT, execute_right },
    { NODE_COLOR, execute_color },
    { NODE_REPEAT, execute_repeat },
};

static int functions_count = sizeof(functions) / sizeof(struct function);

struct node *
node_new(enum node_type type, int int_value, char *str_value, struct node *subnode)
{

    struct node *node = cast_malloc(struct node);

	node->type = type;
	node->int_value = int_value;
	if (str_value)
		strncpy(node->str_value, str_value, sizeof(node->str_value)/sizeof(char));
    
	node->subnode = subnode;
	node->next = NULL;
    
    for (int i = 0; i < functions_count; i++) {
        if (functions[i].node_type == type) {
            node->execute = functions[i].f;
        }
    }

	return node;
}

void
node_free(struct node *node)
{
	struct node *next = node->next;
	struct node *subnode = node->subnode;

	free(node);

	if (next) {
		node_free(next);
	}

	if (subnode) {
		node_free(subnode);
	}
}

void
node_append(struct node *node, struct node *new)
{
	struct node *current = node;
    
	while (current->next) {
		current = current->next;
	}

	current->next = new;
}

void
node_print(struct node *node)
{
	struct node *cur = node;

	while (cur) {
	
		switch (cur->type) {
			case NODE_FORWARD:
				printf("FORWARD");
				break;
			case NODE_LEFT:
				printf("LEFT");
				break;
			case NODE_RIGHT:
				printf("RIGHT");
				break;
			case NODE_REPEAT:
				printf("REPEAT");
				break;
		}

		printf(" %d", cur->int_value);
		if (cur->type == NODE_REPEAT) {
			printf(" [\n");
			node_print(cur->subnode);
			printf("]");
		}
		printf("\n");

		cur = cur->next;
	}
}
