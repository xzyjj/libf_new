#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/crc32.h>
#include <libf/ds/swisstable.h>


#undef SIZE
#define SIZE (SWISSTABLE_ALIGN(1 << 21))
#undef NOSIZE
#define NOSIZE (100000)

struct T {
	char key[12];
};

static int32 _cmp(void *a, const void *b, uint64 len) {
	return XSYMBOL(memcmp)(((struct T *)a)->key, (char *)b, len);
}

static uint64L _hash(const void *a, uint64 len) {
#if 0

	unsigned long hash = 5381;
	while (len--)
		hash = ((hash << 5) + hash)
			+ ((char *)a)[len]; /* hash * 33 + c */

	return hash;

#else

	unsigned long hash = (uint64L)FSYMBOL(crc32)((uint8 *)a, len,
		CRC32_DEFAULT_MSB_TYPE);

	return hash | (hash ^ 0x123456789) << 32;

#endif
}

int main(void) {
	struct swisstable_group *ctrl = malloc(
		sizeof(struct swisstable_group) * SIZE);
	struct T *array = malloc(
		sizeof(struct T) * SIZE);
	struct T *p, p2;
	clock_t start, end;
	double time;

	XSYMBOL(memset)(ctrl, SWISSTABLE_EMPTY,
		sizeof(struct swisstable_group) * SIZE);
	SWISSTABLE_NEW(head, ctrl, array, sizeof(struct T),
		SIZE,
		_hash, _cmp);

	/* insert */
	start = clock();
	for (int32 i = 0; i < SIZE - NOSIZE; i++) {
		((int32 *)p2.key)[0] = i;
		((int32 *)p2.key)[1] = ~i;
		((int32 *)p2.key)[3] = i;
		p = FSYMBOL(swisstable_insert)(&head, p2.key, 12);
		if (p) {
			XSYMBOL(memcpy)(p->key, p2.key, 12);
		} else {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d (%lu:%lu%%), time: %.6fs (%.2f/s), %.2fns/op\n",
		SIZE, head.size, SWISSTABLE_FACTOR(&head),
		time, (double)SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / (SIZE - NOSIZE));

	/* find */
	start = clock();
	for (int32 i = 0; i < SIZE - NOSIZE; i++) {
		((int32 *)p2.key)[0] = i;
		((int32 *)p2.key)[1] = ~i;
		((int32 *)p2.key)[3] = i;
		p = FSYMBOL(swisstable_find)(&head, p2.key, 12);
		if (!p) {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("find: %d (%lu:%lu%%), time: %.6fs (%.2f/s), %.2fns/op\n",
		SIZE, SWISSTABLE_SIZE(&head), SWISSTABLE_FACTOR(&head),
		time, (double)SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / (SIZE - NOSIZE));

	struct swisstable_group *ctrl_tmp = malloc(
		sizeof(struct swisstable_group) * SIZE);
	struct T *array_tmp = malloc(
		sizeof(struct T) * SIZE);

	XSYMBOL(memset)(ctrl_tmp, SWISSTABLE_EMPTY,
		sizeof(struct swisstable_group) * SIZE);
	SWISSTABLE_NEW(head_tmp, ctrl_tmp, array_tmp, sizeof(struct T),
		SIZE,
		_hash, _cmp);

	/* resize/insert */
	start = clock();
	for (int32 i = 0; i < SIZE; i++) {
		if (SWISSTABLE_UNUSED(&head, i))
			continue;

		p = FSYMBOL(swisstable_insert)(&head_tmp, array[i].key, 12);
		if (p) {
			XSYMBOL(memcpy)(p->key, array[i].key, 12);
		} else {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("resize/insert: %d (%lu:%lu%%), time: %.6fs (%.2f/s), %.2fns/op\n",
		SIZE, SWISSTABLE_SIZE(&head_tmp), SWISSTABLE_FACTOR(&head_tmp),
		time, (double)SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / (SIZE - NOSIZE));

	/* delete */
	start = clock();
	for (int32 i = 0; i < SIZE - NOSIZE; i++) {
		((int32 *)p2.key)[0] = i;
		((int32 *)p2.key)[1] = ~i;
		((int32 *)p2.key)[3] = i;
		p = FSYMBOL(swisstable_delete)(&head_tmp, p2.key, 12);
		if (!p) {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("delete: %d (%lu:%lu%%), time: %.6fs (%.2f/s), %.2fns/op\n",
		SIZE, SWISSTABLE_SIZE(&head_tmp), SWISSTABLE_FACTOR(&head_tmp),
		time, (double)SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / (SIZE - NOSIZE));

	free(ctrl);
	free(array);

	return 0;
}
