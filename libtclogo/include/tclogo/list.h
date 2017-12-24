#ifndef _TCLOGO_LIST_H_
#define _TCLOGO_LIST_H_

#define list_get(l, t) ((t *) l->data) 
#define list_next(cur) ({ cur = cur->next; })

#define for_each(T, var, list, code) do {       \
        const struct list_head *_cur_ = list;   \
        T *var;                                 \
                                                \
        while (_cur_) {                         \
            var = list_get(_cur_, T);           \
            code;                               \
            list_next(_cur_);                   \
        }                                       \
    } while (0)                                 \

struct list_head {
    void             *data;
    struct list_head *next;
};

struct list_head   *list_new    ();

void                list_add    (struct list_head **head,
                                 void              *data);

void                list_add_all(struct list_head **head,
                                 struct list_head  *src);
                                 
int                 list_get_size(struct list_head *head);

void                list_free   (struct list_head *head);

#endif
