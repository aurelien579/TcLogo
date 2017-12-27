#include <tclogo/node.h>
#include <tclogo/list.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void node_tests()
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
	
	printf("node_tests success!\n");
}

void list_tests()
{
    struct list_head *head = list_new();
    list_append(&head, 4);
    list_append(&head, 5);
    
    assert(head->data == 4);
    assert(head->next->data == 5);
    assert(list_get_size(head) == 2);
    
    printf("list_tests success!\n");    
}

int main()
{
    node_tests();
    list_tests();
	
	return 0;
}
