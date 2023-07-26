/**
 * File: list.h
 * Author: ZhuXindi
 * Date: 2023-07-10
 */

#ifndef _LIST_H
#define _LIST_H

#include <cbase/utils.h>

/* doubly linked list */
struct list {
	struct list *next; /* next node */
	struct list *prev; /* previous node */
};

#define LIST_INIT(name) { &(name), &(name) }

#define DEFINE_LIST(name) \
	struct list name = LIST_INIT(name)

static inline void list_init(struct list *list)
{
	list->next = list;
	list->prev = list;
}

/* add a new entry */
static inline void __list_add(struct list *new,
			      struct list *prev,
			      struct list *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

static inline void list_add(struct list *new, struct list *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list *new, struct list *head)
{
	__list_add(new, head->prev, head);
}

/* delete entry from list */
static inline void __list_del(struct list *prev, struct list *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(struct list *entry)
{
	__list_del(entry->prev, entry->next);
	list_init(entry);
}

/* replace old entry by new one */
static inline void __list_replace(struct list *old, struct list *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_replace(struct list *old, struct list *new)
{
	__list_replace(old, new);
	list_init(old);
}

/* delete from one list and add into another */
static inline void list_move(struct list *list, struct list *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

static inline void list_move_tail(struct list *list, struct list *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

/* test whether a list is empty */
static inline int list_is_empty(const struct list *head)
{
	return head->next == head;
}

/* test whether a list has just one entry */
static inline int list_is_singular(const struct list *head)
{
	return !list_is_empty(head) && (head->next == head->prev);
}

/* cut a list into two */
static inline void __list_cut(struct list *list,
			      struct list *head,
			      struct list *entry)
{
	struct list *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

static inline void list_cut(struct list *list,
			    struct list *head,
			    struct list *entry)
{
	if (list_is_empty(head))
		return;
	if (list_is_singular(head) &&
	    (head->next != entry && head != entry))
		return;
	if (entry == head)
		list_init(list);
	else
		__list_cut(list, head, entry);
}

/* join two lists */
static inline void __list_splice(const struct list *list,
				 struct list *prev,
				 struct list *next)
{
	struct list *first = list->next;
	struct list *last = list->prev;
	first->prev = prev;
	prev->next = first;
	last->next = next;
	next->prev = last;
}

static inline void list_splice(struct list *list, struct list *head)
{
	if (!list_is_empty(list)) {
		__list_splice(list, head, head->next);
		list_init(list);
	}
}

static inline void list_splice_tail(struct list *list, struct list *head)
{
	if (!list_is_empty(list)) {
		__list_splice(list, head->prev, head);
		list_init(list);
	}
}

/* entries */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)

/* iterators */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_reverse(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; \
	     pos != (head); \
	     pos = n, n = pos->next)

#define list_for_each_safe_reverse(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

#define list_for_each_entry(pos, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_reverse(pos, head, member) \
	for (pos = list_entry((head)->prev, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_entry(pos->member.prev, typeof(*pos), member))

#define list_for_each_entry_continue(pos, head, member) \
	for (pos = list_entry(pos->member.next, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_continue_reverse(pos, head, member) \
	for (pos = list_entry(pos->member.prev, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_entry(pos->member.prev, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_entry((head)->next, typeof(*pos), member), \
		n = list_entry(pos->member.next, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))

#define list_for_each_entry_safe_reverse(pos, n, head, member) \
	for (pos = list_entry((head)->prev, typeof(*pos), member), \
		n = list_entry(pos->member.prev, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = n, n = list_entry(n->member.prev, typeof(*n), member))

#endif /* _LIST_H */
