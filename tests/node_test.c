#include "node.h"
#include <stdlib.h>
#include <assert.h>

int main()
{
	prog_t p = prog_new(NODE_FORWARD, 10, 0, 0);
	prog_append(p, prog_new(NODE_RIGHT, 5, 0, 0));
	
	prog_t root = prog_new(NODE_LEFT, 6, 0, 0);
	prog_append(root, prog_new(NODE_REPEAT, 5, 0, p));
	
	assert(root->next->subprog == p);
	assert(root->int_value = 6);
	assert(root->next->subprog->int_value == 10);
	assert(root->next->subprog->next->int_value == 5);
	
	printf("Success!\n");
	
	return 0;
}
