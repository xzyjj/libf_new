#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xstdlib.h>
#include <libf/ds/list.h>


#undef NODE_SIZE
#define NODE_SIZE (1000000)

struct T {
	int32 key;
	struct list node;
};

int main(void) {
	LIST_NEW(head);
	struct T *T = malloc(sizeof(struct T) * NODE_SIZE);
	clock_t start, end;
	float64 time;

	RANDOM_TYPE0_NEW(rand, 123456);
	for (int32 i = 0; i < NODE_SIZE; i++) {
		XSYMBOL(random_r)(&rand, &T[i].key);
	}

	/* add */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(list_add)(&head, &T[i].node);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("add: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* del */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(list_del)(&head, &T[i].node);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("del: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* add tail */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(list_add_tail)(&head, &T[i].node);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("add_tail: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* swap */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(list_swap)(&head, &T[0].node, &T[i].node);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("swap: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	return 0;
}
