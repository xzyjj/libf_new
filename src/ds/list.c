/* list.c - linked list implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/list.h>


/* @func: list_add - linked list add
* @param1: struct list_head * # list head
* @param2: struct list *      # new list
* @return: struct list *      # return new list
*/
struct list *FSYMBOL(list_add)(struct list_head *head,
		struct list *new) {
	struct list *node = head->node;
	if (node) {
		new->prev = node->prev ? node->prev : node;
		new->next = node;
		node->prev = new;
	} else {
		new->prev = new->next = NULL;
	}
	head->node = new;

	return new;
} /* end */

/* @func: list_add_tail - linked list add (add to tail)
* @param1: struct list_head * # list head
* @param2: struct list *      # new list
* @return: struct list *      # return new list
*/
struct list *FSYMBOL(list_add_tail)(struct list_head *head,
		struct list *new) {
	struct list *node = head->node, *prev = NULL;
	if (node) {
		prev = node->prev ? node->prev : node;
		new->prev = prev;
		new->next = NULL;
		node->prev = prev->next = new;
	} else {
		new->prev = new->next = NULL;
		head->node = new;
	}

	return new;
} /* end */

/* @func: list_del - linked list delete
* @param1: struct list_head * # list head
* @param2: struct list *      # old list
* @return: struct list *      # return old list
*/
struct list *FSYMBOL(list_del)(struct list_head *head,
		struct list *old) {
	struct list *node = head->node,
		*prev = old->prev, *next = old->next;
	if (node == old) {
		if (next)
			next->prev = (prev != next) ? prev : NULL;
		head->node = next;
	} else {
		prev->next = next ? next : NULL;
		if (next) {
			next->prev = prev;
		} else {
			node->prev = prev;
		}
	}

	return old;
} /* end */

/* @func: list_insert - linked list insert
* @param1: struct list_head * # list head
* @param2: struct list *      # list node
* @param3: struct list *      # new list
* @return: struct list *      # return new list
*/
struct list *FSYMBOL(list_insert)(struct list_head *head,
		struct list *node, struct list *new) {
	struct list *prev = node->prev;
	if (prev) {
		new->prev = prev;
		new->next = node;
		node->prev = new;
		if (node != head->node) {
			prev->next = new;
		} else {
			head->node = new;
		}
	} else {
		FSYMBOL(list_add)(head, new);
	}

	return new;
} /* end */

/* @func: list_insert_tail - linked list insert (add to tail)
* @param1: struct list_head * # list head
* @param2: struct list *      # list node
* @param3: struct list *      # new list
* @return: struct list *      # return new list
*/
struct list *FSYMBOL(list_insert_tail)(struct list_head *head,
		struct list *node, struct list *new) {
	struct list *next = node->next;
	if (next) {
		new->next = next;
		new->prev = node;
		node->next = next->prev = new;
	} else {
		FSYMBOL(list_add_tail)(head, new);
	}

	return new;
} /* end */

/* @func: list_swap - linked list swap
* @param1: struct list_head * # list head
* @param2: struct list *      # old list
* @param3: struct list *      # new list
* @return: void
*/
void FSYMBOL(list_swap)(struct list_head *head,
		struct list *old, struct list *new) {
	if (old->next == new) {
		FSYMBOL(list_del)(head, new);
		FSYMBOL(list_insert)(head, old, new);
	} else if (old->prev == new) {
		FSYMBOL(list_del)(head, new);
		FSYMBOL(list_insert_tail)(head, old, new);
	} else {
		struct list *node = head->node;
		node = (node == new) ? node->next : new->next;

		FSYMBOL(list_del)(head, new);
		FSYMBOL(list_insert_tail)(head, old, new);

		FSYMBOL(list_del)(head, old);
		FSYMBOL(list_insert)(head, node, old);
	}
} /* end */
