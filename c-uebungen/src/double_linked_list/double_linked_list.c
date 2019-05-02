#include "./double_linked_list.h"
#include <assert.h>
#include <stdio.h>


/*initialize "shortcut links" for empty list*/
extern void
list_init(struct list_head *head) 
{   
    head->prev = head;
    head->next = head;
}

/*insert new entry after the specified head*/
extern void
list_add(struct list_head *new, struct list_head *head) {
    new->next = head->next;    
    new->prev = head;
    head->next = new;
}

/*insert new entry before the specified head*/
extern void
list_add_tail(struct list_head *new, struct list_head *head) {
    new->prev = head->prev;    
    new->next = head;
    head->prev = new;
}


/*deletes entry from list, reinitializes it (next = prev = 0),
and returns pointer to entry*/
extern struct list_head*
list_del(struct list_head *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;

    entry->next = 0;
    entry->prev = 0;
    return entry;
}


/*delete entry from one list and insert after the specified head*/
extern void
list_move(struct list_head *entry, struct list_head *head) {
    list_add(list_del(entry), head);
}

/*delete entry from one list and insert before the specified head*/
extern void
list_move_tail(struct list_head *entry, struct list_head *head) {
    list_add_tail(list_del(entry), head);
}

/*tests whether a list is empty*/
extern int
list_empty(struct list_head *head) {
    return head == head->prev && head == head->next;
};


int main() {

    struct list_head *anchor;
    anchor = malloc(sizeof(struct list_head));
    list_init(anchor);

    struct list_elem *e1, *e2, e3, e4, e5, e6;
    e1 = malloc(sizeof(struct list_elem));
    e1->pid = 1;
    e2 = malloc(sizeof(struct list_elem));
    e2->pid = 2;
    e3.pid = 3;
    e4.pid = 4;
    e5.pid = 5;
    e6.pid = 6;

    list_add(&e1->head, anchor);
    list_add(&e2->head, anchor);
    list_add(&e3.head, anchor);
    list_add(&e4.head, anchor);
    list_add(&e5.head, anchor);
    //list_add((struct list_head*) &e6, (struct list_head*) &e5);

    //list_add_tail((struct list_head*) &e6, (struct list_head*) &e3);

    list_del(&e1->head);

    struct list_head *current = anchor->next;

    for (int i = 0; i < 5; i++) {
        struct list_elem* elem= (struct list_elem*) current;
        printf("%d\n", elem->pid);
        current = current->next;
    }

    printf("success");

}


