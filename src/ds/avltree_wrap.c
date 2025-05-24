/* avltree_wrap.c - avl-tree wrap implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/avltree.h>


/* @func: avl_insert_wrap - avl-tree node insert
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # new node
* @param3: int32 (*)(void *, void *) # cmp callback
* @return: struct avl_node * # return new node / NULL pointer
*/
struct avl_node *FSYMBOL(avl_insert_wrap)(struct avl_root *root,
		struct avl_node *new, int32 (*cmp)(void *, void *)) {
	struct avl_node **link = &root->node, *parent = NULL;
	while (*link) {
		parent = *link;
		int32 m = cmp(parent, new);
		if (m < 0) {
			link = &parent->left;
		} else if (m > 0) {
			link = &parent->right;
		} else {
			return NULL;
		}
	}

	new->left = new->right = NULL;
	new->parent = parent;
	new->bf = 1;
	*link = new;
	FSYMBOL(avl_balance)(root, new);

	return new;
} /* end */

/* @func: avl_search_wrap - avl-tree node search
* @param1: struct avl_root * # root node
* @param2: void *            # callback arg
* @param3: int32 (*)(void *, void *) # cmp callback
* @return: struct avl_node * # return search node / NULL pointer
*/
struct avl_node *FSYMBOL(avl_search_wrap)(struct avl_root *root,
		void *arg, int32 (*cmp)(void *, void *)) {
	struct avl_node **link = &root->node, *parent = NULL;
	while (*link) {
		parent = *link;
		int32 m = cmp(parent, arg);
		if (m < 0) {
			link = &parent->left;
		} else if (m > 0) {
			link = &parent->right;
		} else {
			return parent;
		}
	}

	return NULL;
} /* end */

/* @func: avl_erase_wrap - avl-tree node erase
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # old node
* @return: struct avl_node * # return erase node / NULL pointer
*/
struct avl_node *FSYMBOL(avl_erase_wrap)(struct avl_root *root,
		struct avl_node *old) {
	if (old)
		FSYMBOL(avl_erase_fix)(root, old);

	return old;
} /* end */

/* @func: avl_erase2_wrap - avl-tree node search
* @param1: struct avl_root * # root node
* @param2: void *            # callback arg
* @param3: int32 (*)(void *, void *) # cmp callback
* @return: struct avl_node * # return erase node / NULL pointer
*/
struct avl_node *FSYMBOL(avl_erase2_wrap)(struct avl_root *root,
		void *arg, int32 (*cmp)(void *, void *)) {
	struct avl_node *old = FSYMBOL(avl_search_wrap)(root, arg, cmp);
	if (old)
		FSYMBOL(avl_erase_fix)(root, old);

	return old;
} /* end */
