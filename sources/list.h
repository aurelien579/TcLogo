#ifndef LINE_H
#define LINE_H

#define list_get(l, t) ((t *) l->data) 
#define list_next(cur) ({ cur = cur->next; })

struct list_head {
	void *             data;
	struct list_head * next;
};

struct list_head *list_new();
void list_add(struct list_head **head, void *data);
void list_free(struct list_head *head);

#endif
