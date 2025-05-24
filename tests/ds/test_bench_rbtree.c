#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/sl/xrand.h>
#include <libf/ds/rbtree.h>


#undef NODE_SIZE
#define NODE_SIZE (1000000)

struct T {
	int32 key;
	struct rb_node node;
};

static int32 _cmp(void *node, void *new) {
	int32 key = container_of(node, struct T, node)->key;
	int32 new_key = container_of(new, struct T, node)->key;

	return ((new_key > key) ? 1 : ((new_key < key) ? -1 : 0));
}

static int32 _search_cmp(void *node, void *arg) {
	int32 key = container_of(node, struct T, node)->key;
	int32 s_key = *((int32 *)arg);

	return ((s_key > key) ? 1 : ((s_key < key) ? -1 : 0));
}

int main(void) {
	RB_ROOT_NEW(root);
	struct T *T = malloc(sizeof(struct T) * NODE_SIZE);
	clock_t start, end;
	float64 time;

	RANDOM_TYPE0_NEW(rand, 123456);
	for (int32 i = 0; i < NODE_SIZE; i++) {
#if 1

		XSYMBOL(random_r)(&rand, &T[i].key);

#else

		T[i].key = i;

#endif
	}

	/* insert */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(rb_insert_wrap)(&root, &T[i].node, _cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* search */
	start = clock();
	for (int32 i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(rb_search_wrap)(&root, &T[i].key, _search_cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* erase */
	start = clock();
	for (int32 i = 0; i < (NODE_SIZE / 10); i++) {
		FSYMBOL(rb_erase2_wrap)(&root, &T[i].node, _cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE / 10, time, (float64)(NODE_SIZE / 10) / time);

	/* search */
	start = clock();
	for (int32 i = (NODE_SIZE / 10); i < ((NODE_SIZE / 10) + 10); i++) {
		printf("search: %d %p %p\n", T[i - 1].key, &T[i - 1].node,
			FSYMBOL(rb_search_wrap)(&root, &T[i - 1].node, _cmp));
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs (%.2f/s)\n",
		10, time, (float64)10 / time);

	/* search */
	start = clock();
	for (int i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(rb_search_wrap)(&root, &T[i].node, _search_cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	/* erase */
	start = clock();
	for (int i = 0; i < NODE_SIZE; i++) {
		FSYMBOL(rb_erase2_wrap)(&root, &T[i].node, _cmp);
	}
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs (%.2f/s)\n",
		NODE_SIZE, time, (float64)NODE_SIZE / time);

	return 0;
}
