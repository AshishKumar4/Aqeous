
#include "list.h"

void list_remove(ListNode *node)
{
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
}

void list_insert_after(ListNode *node, ListNode *after)
{
	node->next = after->next;
	node->prev = after;
	after->next = node;
	if (node->next)
		node->next->prev = node;
}

void list_insert_before(ListNode *node, ListNode *before)
{
	node->prev = before->prev;
	node->next = before;
	before->prev = node;
	if (node->prev)
		node->prev->next = node;
}
