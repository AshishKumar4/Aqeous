
#ifndef __BASE_LIST_H__
#define __BASE_LIST_H__

#include <stddef.h>
#include <stdint.h>

#define container_of(ptr, type, member) ({                         \
	const typeof(((type *)0)->member) *__mptr = (ptr);         \
	(type *)((uint8_t *)__mptr - offsetof(type, member) );})   \

typedef struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
} ListNode;

// Remove ListNode node from the doubly linked list it's a part of.
void list_remove(ListNode *node);
// Insert ListNode node after ListNode after in a doubly linked list.
void list_insert_after(ListNode *node, ListNode *after);
// Insert ListNode node before ListNode before in a doubly linked list.
void list_insert_before(ListNode *node, ListNode *before);

#define list_for_each(ptr, head, member)                                \
	for ((ptr) = container_of((head).next, typeof(*(ptr)), member); \
		&((ptr)->member);                                       \
		(ptr) = container_of((ptr)->member.next,                \
			typeof(*(ptr)), member))

#endif /* __BASE_LIST_H__ */
