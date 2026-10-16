#pragma once
#include <stddef.h>
struct list_head { struct list_head *next, *prev; };
#define LIST_HEAD(name) struct list_head name = { &(name), &(name) }
#define INIT_LIST_HEAD(head) do { (head)->next=(head); (head)->prev=(head); } while (0)
#define container_of(ptr,type,member) ((type *)((char *)(ptr) - offsetof(type,member)))
#define list_entry(ptr,type,member) container_of(ptr,type,member)
#define list_first_entry(head,type,member) list_entry((head)->next,type,member)
#define list_next_entry(pos,member) list_entry((pos)->member.next,__typeof__(*(pos)),member)
#define list_for_each_entry(pos,head,member) \
 for (pos=list_first_entry(head,__typeof__(*pos),member); &(pos)->member!=(head); pos=list_next_entry(pos,member))
#define list_for_each_entry_safe(pos,n,head,member) \
 for (pos=list_first_entry(head,__typeof__(*pos),member),n=list_next_entry(pos,member); \
      &(pos)->member!=(head); pos=n,n=list_next_entry(n,member))
static inline void list_add_tail(struct list_head *n, struct list_head *head) {
 n->prev=head->prev; n->next=head; head->prev->next=n; head->prev=n;
}
static inline void list_del(struct list_head *n) { n->next->prev=n->prev; n->prev->next=n->next; }
static inline int list_empty(const struct list_head *h) { return h->next==h; }
