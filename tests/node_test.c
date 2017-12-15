#include <tclogo/node.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main()
{
	struct node *n1, *n2, *n3, *n4, *n5;
	
	n1 = node_new(NODE_FORWARD, 1);
	node_set_int(n1, 0, 45);
	
	n2 = node_new(NODE_REPEAT, 2);
	node_set_int(n2, 0, 5);
	node_set_subnode(n2, 1, n1);
	
	n3 = node_new(NODE_RIGHT, 1);
	node_set_int(n3, 0, 6);
	
	n4 = node_new(NODE_FORWARD, 1);
	node_set_int(n4, 0, 7);
	
	node_append(n4, n1);
	node_append(n4, n2);
	node_append(n4, n3);
	
	assert(node_get_int(n4, 0) == 7);
	assert(node_get_int(n3, 0) == 6);
	assert(node_get_int(n2, 0) == 5);
	assert(node_get_subnode(n2, 1) == n1);
	assert(node_get_int(n1, 0) == 45);
	
	assert(n4->next == n1);
	assert(n4->next->next == n2);
	assert(n4->next->next->next == n3);
	
	printf("Success!\n");
	
	return 0;
}
