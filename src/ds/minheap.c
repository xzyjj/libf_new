/* minheap.c - minheap data structure implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/minheap.h>


/* @func: _minheapify_swap (static) - minheap member swap
* @param1: void ** # member pointer
* @param2: void ** # member pointer
* @return: void
*/
static void _minheapify_swap(void **a, void **b) {
	void *t = *a;
	*a = *b;
	*b = t;
} /* end */

/* @func: _minheapify_up (static) - minheap member up shift
* @param1: struct minheap_head * # minheap head
* @param2: uint64                # array index
* @param3: int32 (*)(void *, void *) # callback
* @return: void
*/
static void _minheapify_up(struct minheap_head *head, uint64 n,
		int32 (*cmp)(void *, void *)) {
	while (n > 0) {
		uint64 k = MINHEAP_PARENT(n);
		if (!(cmp(head->array[k], head->array[n]) > 0))
			break;

		_minheapify_swap(&head->array[k], &head->array[n]);
		n = k;
	}
} /* end */

/* @func: _minheapify_down (static) - minheap member down shift
* @param1: struct minheap_head * # minheap head
* @param2: uint64                # array index
* @param3: int32 (*)(void *, void *) # callback
* @return: void
*/
static void _minheapify_down(struct minheap_head *head, uint64 n,
		int32 (*cmp)(void *, void *)) {
	uint64 m, l, r;
	while (1) {
		m = n;
		l = MINHEAP_LEFT(n);
		r = MINHEAP_RIGHT(n);

		if (l < head->size && cmp(head->array[l], head->array[m]) < 0)
			m = l;
		if (r < head->size && cmp(head->array[r], head->array[m]) < 0)
			m = r;
		if (m == n)
			break;

		_minheapify_swap(&head->array[n], &head->array[m]);
		n = m;
	}
} /* end */

/* @func: minheap_insert - insert a minheap array member
* @param1: struct minheap_head * # minheap head
* @param2: void *                # new member pointer
* @param3: int32 (*)(void *, void *) # callback
* @return: int32                 # 0: no error, -1: error array full
*/
int32 FSYMBOL(minheap_insert)(struct minheap_head *head, void *new,
		int32 (*cmp)(void *, void *)) {
	if (head->size >= head->total_size)
		return -1;

	head->array[head->size] = new;
	_minheapify_up(head, head->size, cmp);
	head->size++;

	return 0;
} /* end */

/* @func: minheap_build - build a minheap (array assignment)
* @param1: struct minheap_head * # minheap head
* @param2: int32 (*)(void *, void *) # callback
* @return: void
*/
void FSYMBOL(minheap_build)(struct minheap_head *head,
		int32 (*cmp)(void *, void *)) {
	if (head->size < 1)
		return;

	int64 n = MINHEAP_PARENT(head->size - 1);
	for (; n >= 0; n--)
		_minheapify_down(head, n, cmp);
} /* end */

/* @func: minheap_search - search for minheap array member index
* @param1: struct minheap_head * # minheap head
* @param2: void *                # member pointer
* @return: int64                 # array index (-1: not found)
*/
int64 FSYMBOL(minheap_search)(struct minheap_head *head, void *p) {
	for (uint64 i = 0; i < head->size; i++) {
		if (head->array[i] == p)
			return i;
	}

	return -1;
} /* end */

/* @func: minheap_search2 - search for minheap array member index
* @param1: struct minheap_head * # minheap head
* @param2: void *                # callback arg
* @param3: int32 (*)(void *, void *) # callback
* @return: int64                 # array index (-1: not found)
*/
int64 FSYMBOL(minheap_search2)(struct minheap_head *head, void *arg,
		int32 (*cmp)(void *, void *)) {
	for (uint64 i = 0; i < head->size; i++) {
		if (!cmp(head->array[i], arg))
			return i;
	}

	return -1;
} /* end */

/* @func: minheap_erase - erase a minheap array member
* @param1: struct minheap_head * # minheap head
* @param2: uint64                # array index
* @param3: int32 (*)(void *, void *) # callback
* @return: int32                 # 0: no error, -1: error array full
*/
int32 FSYMBOL(minheap_erase)(struct minheap_head *head, uint64 ind,
		int32 (*cmp)(void *, void *)) {
	if (head->size < 1 || ind > (head->size - 1))
		return -1;

	head->array[ind] = head->array[head->size - 1];
	head->size--;

	uint64 k = MINHEAP_PARENT(ind);
	if (ind > 0 && cmp(head->array[ind], head->array[k]) < 0) {
		_minheapify_up(head, ind, cmp);
	} else if (head->size) {
		_minheapify_down(head, ind, cmp);
	}

	return 0;
} /* end */

/* @func: minheap_extract - extract a minheap minimum member
* @param1: struct minheap_head * # minheap head
* @param2: int32 (*)(void *, void *) # callback
* @return: void *                # minimum member / NULL pointer
*/
void* FSYMBOL(minheap_extract)(struct minheap_head *head,
		int32 (*cmp)(void *, void *)) {
	if (head->size < 1)
		return NULL;

	void *p = head->array[0];
	head->array[0] = head->array[head->size - 1];
	head->size--;

	if (head->size)
		_minheapify_down(head, 0, cmp);

	return p;
} /* end */
