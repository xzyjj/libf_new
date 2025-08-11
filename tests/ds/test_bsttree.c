#include <stdio.h>
#include <stdlib.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bsttree.h>


struct T {
	int32 key;
	struct bst_node node;
};

static void _each(struct bst_node *node, int32 n, int32 c) {
	if (node) {
		_each(node->right, n + 4, 1);

		struct T *bst = container_of(node, struct T, node);
		for (int32 i = 0; i < n; i++)
			printf(" ");
		if (c == 1) {
			printf("/%d (l: %p, r: %p, n: %p)\n",
				bst->key,
				node->left, node->right, node);
		} else if (c == -1) {
			printf("\\%d (l: %p, r: %p, n: %p)\n",
				bst->key,
				node->left, node->right, node);
		} else {
			printf("=%d (l: %p, r: %p, n: %p)\n",
				bst->key,
				node->left, node->right, node);
		}

		_each(node->left, n + 4, -1);
	} else {
		for (int32 i = 0; i < n; i++)
			printf(" ");
		if (c == 1) {
			printf("/Nil\n");
		} else if (c == -1) {
			printf("\\Nil\n");
		} else {
			printf("=Nil\n");
		}
	}
}

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
	BST_ROOT_NEW(root);
	struct T *bst = NULL;

	int32 r = 0, t = 0;
	for (int32 i = 0; i < 1024; i++) {
e:
		if (t == 0) {
			printf("insert> ");
		} else {
			printf("erase> ");
		}

		fflush(stdout);
		if (scanf("%d", &r) < 0)
			break;

		printf("\n> %d\n\n", r);
		if (r == -9990) {
			t = 0;
			goto e;
		} else if (r == -9999) {
			t = 2;
			goto e;
		} else if (r == -99999) {
			break;
		}

		if (t == 0) {
			bst = malloc(sizeof(struct T));
			bst->key = r;
			FSYMBOL(bst_insert)(&root, &bst->node, _cmp);
		} else {
			void *p = FSYMBOL(bst_erase)(&root, &r, _search_cmp);
			if (p)
				free(container_of(p, struct T, node));
		}
		_each(root.node, 0, 0);
	}

	return 0;
}
