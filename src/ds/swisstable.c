/* swisstable.c - swiss hash table implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/swisstable.h>


/* @func: swisstable_insert - swisstable insert function
* @param1: struct swisstable_head * # swisstable head
* @param2: const void *             # input key
* @param3: uint64                   # key length
* @return: void *                   # swisstable member / NULL pointer
*/
void *FSYMBOL(swisstable_insert)(struct swisstable_head *head,
		const void *key, uint64 len) {
	uint64L h = head->call_hash(key, len);
	uint64L h1 = h >> 7;
	uint8 h2 = h & 0x7f;

	uint64 n = head->total_size;
	uint64 m = h1 % n;
	for (uint64 i = 0; i < n; i++) {
		uint64 pos = (m + i) % n;

		void *p = (void *)((char *)head->array + (head->wsize * pos));
		if (head->group[pos].ctrl == SWISSTABLE_EMPTY
				|| head->group[pos].ctrl == SWISSTABLE_DELETE) {
			head->group[pos].ctrl = h2;
			head->size++;
			return p;
		}
		if (head->group[pos].ctrl == h2
				&& !head->call_cmp(p, key, len))
			return p;
	}

	return NULL;
} /* end */

/* @func: swisstable_find - swisstable find function
* @param1: struct swisstable_head * # swisstable head
* @param2: const void *             # input key
* @param3: uint64                   # key length
* @return: void *                   # swisstable member / NULL pointer
*/
void *FSYMBOL(swisstable_find)(struct swisstable_head *head,
		const void *key, uint64 len) {
	uint64L h = head->call_hash(key, len);
	uint64L h1 = h >> 7;
	uint8 h2 = h & 0x7f;

	uint64 n = head->total_size;
	uint64 m = h1 % n;
	for (uint64 i = 0; i < n; i++) {
		uint64 pos = (m + i) % n;
		if (head->group[pos].ctrl != h2)
			continue;
		if (head->group[pos].ctrl == SWISSTABLE_EMPTY)
			break;

		void *p = (void *)((char *)head->array + (head->wsize * pos));
		if (!head->call_cmp(p, key, len))
			return p;
	}

	return NULL;
} /* end */

/* @func: swisstable_delete - swisstable delete function
* @param1: struct swisstable_head * # swisstable head
* @param2: const void *             # input key
* @param3: uint64                   # key length
* @return: void *                   # swisstable member / NULL pointer
*/
void *FSYMBOL(swisstable_delete)(struct swisstable_head *head,
		const void *key, uint64 len) {
	uint64L h = head->call_hash(key, len);
	uint64L h1 = h >> 7;
	uint8 h2 = h & 0x7f;

	uint64 n = head->total_size;
	uint64 m = h1 % n;
	for (uint64 i = 0; i < n; i++) {
		uint64 pos = (m + i) % n;
		if (head->group[pos].ctrl != h2)
			continue;
		if (head->group[pos].ctrl == SWISSTABLE_EMPTY)
			break;

		void *p = (void *)((char *)head->array + (head->wsize * pos));
		if (!head->call_cmp(p, key, len)) {
			head->group[pos].ctrl = SWISSTABLE_DELETE;
			head->size--;
			return p;
		}
	}

	return NULL;
} /* end */
