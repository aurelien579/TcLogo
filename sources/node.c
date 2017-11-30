#include "node.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node *
node_new(int type, int int_value, char *str_value, struct node *subnode,
         void (*execute) (struct state*, const struct node*))
{
	struct node *node = cast_malloc(struct node);

	node->type = type;
	node->int_value = int_value;
	if (str_value)
		strncpy(node->str_value, str_value, sizeof(node->str_value)/sizeof(char));
    
	node->subnode = subnode;
	node->next = NULL;
    node->execute = execute;

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
