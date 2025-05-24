#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xrand.h>
#include <libf/ds/minheap.h>


#undef NODE_SIZE
#define NODE_SIZE (100000)

static void _each(struct minheap_head *head, int32 ind, int32 n, int32 c) {
	if (ind >= (int32)head->size)
		return;

	_each(head, MINHEAP_RIGHT(ind), n + 4, 1);

	for (int i = 0; i < n; i++)
		printf(" ");

	int32 *key = head->array[ind];
	if (c == 1) {
		printf("/%d (%d)\n", *key, ind);
	} else if (c == -1) {
		printf("\\%d (%d)\n", *key, ind);
	} else {
		printf("=%d (%d)\n", *key, ind);
	}

	_each(head, MINHEAP_LEFT(ind), n + 4, -1);
}

static int32 _cmp(void *a, void *b) {
	int32 _a = *((int32 *)a), _b = *((int32 *)b);

	return ((_a > _b) ? 1 : ((_a < _b) ? -1 : 0));
}

int main(void) {
	int32 *array = malloc(MINHEAP_SIZEOF * NODE_SIZE);
	int32 *arry = malloc(sizeof(int32) * NODE_SIZE);
	MINHEAP_NEW(head, (void **)array, NODE_SIZE);
	clock_t start, end;
	float64 time;

	RANDOM_TYPE0_NEW(rand, 123456);
	for (int32 i = 0; i < NODE_SIZE; i++) {
#if 1

		XSYMBOL(random_r)(&rand, &arry[i]);

#else

		arry[i] = i;

#endif
	}

	/* insert */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(minheap_insert)(&head, &arry[NODE_SIZE - 1 - i], _cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* erase */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE - 10; i++) {
		FSYMBOL(minheap_erase)(&head, 0, _cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE - 10, time, (float64)(NODE_SIZE - 10) / time);

	/* print tree */
	for (uint64 i = 0; i < head.size; i++)
		printf("%d\n", *((int32 *)(head.array[i])));
	_each(&head, 0, 0, 0);

	/* etract */
	uint64 n = head.size;
	for (uint64 i = 0; i < n; i++) {
		void *p = FSYMBOL(minheap_extract)(&head, _cmp);
		printf("%p %d\n", p, *((int32 *)p));
	}

	return 0;
}
