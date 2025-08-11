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
#define SIZE (4 << 20)

struct T {
	char key[12];
};

static int32 _cmp(void *a, const void *b, uint64 len) {
	return XSYMBOL(memcmp)(((struct T *)a)->key, (char *)b, len);
}

static uint64L _hash(const void *a, uint64 len) {
	return (uint64L)FSYMBOL(crc32)((uint8 *)a, len,
			CRC32_DEFAULT_MSB_TYPE)
		| (uint64L)FSYMBOL(crc32)((uint8 *)a,
			len, CRC32_DEFAULT_MSB_TYPE) << 32;
}

int main(void) {
	struct swisstable_group *ctrl = malloc(
		SWISSTABLE_GROUP_ALIGN(SIZE));
	struct T *array = malloc(
		SWISSTABLE_SIZE_ALIGN(sizeof(struct T), SIZE));
	struct T *p, p2;
	clock_t start, end;
	double time;

	XSYMBOL(memset)(ctrl, SWISSTABLE_EMPTY, SIZE);
	SWISSTABLE_NEW(head, ctrl, array, sizeof(struct T),
		SIZE, _hash, _cmp);

	/* insert */
	start = clock();
	for (int32 i = 0; i < SIZE + 1; i++) {
		((int32 *)p2.key)[0] = i;
		((int32 *)p2.key)[1] = ~i;
		((int32 *)p2.key)[3] = i;
		p = FSYMBOL(swisstable_insert)(&head, &p2.key, 12);
		if (p) {
			XSYMBOL(memcpy)(p->key, p2.key, 12);
		} else {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d (%lu), time: %.6fs (%.2f/s)\n",
		SIZE, head.size, time, (double)SIZE / time);

	/* find */
	start = clock();
	for (int32 i = 0; i < SIZE + 1; i++) {
		((int32 *)p2.key)[0] = i;
		((int32 *)p2.key)[1] = ~i;
		((int32 *)p2.key)[3] = i;
		p = FSYMBOL(swisstable_find)(&head, &p2.key, 12);
		if (!p) {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("find: %d (%lu), time: %.6fs (%.2f/s)\n",
		SIZE, head.size, time, (double)SIZE / time);

	/* delete */
	start = clock();
	for (int32 i = 0; i < SIZE + 1; i++) {
		((int32 *)p2.key)[0] = i;
		((int32 *)p2.key)[1] = ~i;
		((int32 *)p2.key)[3] = i;
		p = FSYMBOL(swisstable_delete)(&head, &p2.key, 12);
		if (!p) {
			printf("%d\n", i);
		}
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("delete: %d (%lu), time: %.6fs (%.2f/s)\n",
		SIZE, head.size, time, (double)SIZE / time);

	return 0;
}
