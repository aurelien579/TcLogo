#include <tclogo/node.h>
#include <tclogo/utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct function {
    enum node_type node_type;
    void (*f) (struct logo*, const struct node*);
};

static struct function functions[] = {
    { NODE_FORWARD, execute_forward },
    { NODE_LEFT, execute_left },
    { NODE_RIGHT, execute_right },
    { NODE_COLOR, execute_color },
    { NODE_REPEAT, execute_repeat },
    { NODE_MOVE, execute_move },
    { NODE_SET_ANGLE, execute_set_angle },
    { NODE_GROUP_BEGIN, execute_group_begin },
    { NODE_GROUP_END, execute_group_end },
    { NODE_USE, execute_use },
    { NODE_MOVE_TO, execute_move_to },
    { NODE_RECTANGLE, execute_rectangle }
};

static int functions_count = sizeof(functions) / sizeof(struct function);

struct node *
node_new(enum node_type type,
		 unsigned int 	arg_count)
{
    struct node *node = alloc(struct node);

    node->type = type;
    node->args = alloc_n(struct arg, arg_count);
    node->arg_count = arg_count;
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
    if (node->next) {
        node_free(node->next);
    }

    struct arg arg;

    for (unsigned int i = 0; i < node->arg_count; i++) {
        arg = node->args[i];
        if (arg.type == ARG_NODE) {
            node_free(arg.subnode);
        } else if (arg.type == ARG_STR) {
            free(arg.str);
        }
    }

    free(node->args);
    free(node);
}

void
node_append(struct node *node,
		    struct node *new)
{
    struct node *current = node;

    while (current->next) {
        current = current->next;
    }

    current->next = new;
}

#if 0
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
#endif

int
node_get_int(const struct node *node,
			 unsigned int 		i)
{
    assert(i < node->arg_count);
    return node->args[i].val;
}

char *
node_get_str(const struct node *node,
			 unsigned int 		i)
{
    assert(i < node->arg_count);
    return node->args[i].str;
}

struct node *
node_get_subnode(const struct node *node,
				 unsigned int 		i)
{
    assert(i < node->arg_count);
    return node->args[i].subnode;
}

void
node_set_int(struct node *node,
			 unsigned int i,
			 int 		  val)
{
    assert(i < node->arg_count);
    node->args[i].type = ARG_INT;
    node->args[i].val = val;
}

void
node_set_str(struct node *node,
		     unsigned int i,
		     const char  *str)
{
    assert(i < node->arg_count);
    node->args[i].type = ARG_STR;
    node->args[i].str = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(node->args[i].str, str);
}

void
node_set_subnode(struct node *node,
				 unsigned int i,
				 struct node *subnode)
{
    assert(i < node->arg_count);
    node->args[i].type = ARG_NODE;
    node->args[i].subnode = subnode;
}

