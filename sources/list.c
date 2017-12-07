#include "list.h"
#include "utils.h"
#include <stdlib.h>

struct list_head *
list_new()
{    
    return (struct list_head *) 0;
}

void
list_add(struct list_head **head, void *data)
{
    struct list_head *new = cast_malloc(struct list_head);
    
    new->data = data;
    new->next = *head;
    
    *head = new;
}

void
list_free(struct list_head *head)
{
    struct list_head *tmp;
    struct list_head *cur = head;
    
    while (cur) {
        tmp = cur->next;
        free(cur->data);
        free(cur);
        cur = tmp;
    }
}
