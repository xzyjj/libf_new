/* bsttree.c - binary search tree implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bsttree.h>


/* @func: bst_change_child - change parent node's child
* @param1: struct bst_root * # root node
* @param2: struct bst_node * # parent node
* @param2: struct bst_node * # old node
* @param2: struct bst_node * # new node
* @return: void
*/
void FSYMBOL(bst_change_child)(struct bst_root *root, struct bst_node *parent,
		struct bst_node *old, struct bst_node *new) {
	if (!parent) {
		root->node = new;
	} else if (parent->left == old) {
		parent->left = new;
	} else {
		parent->right = new;
	}
} /* end */

/* @func: bst_insert - bst-tree node insert
* @param1: struct bst_root * # root node
* @param2: struct bst_node * # new node
* @param3: int32 (*)(void *, void *) # callback
* @return: struct bst_node * # return new node / NULL pointer
*/
struct bst_node *FSYMBOL(bst_insert)(struct bst_root *root,
		struct bst_node *new, int32 (*cmp)(void *, void *)) {
	struct bst_node **link = &root->node, *parent = NULL;
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
	*link = new;

	return new;
} /* end */

/* @func: bst_search - bst-tree node search
* @param1: struct bst_root * # root node
* @param2: struct bst_node * # callback arg
* @param3: int32 (*)(void *, void *) # callback
* @return: struct bst_node * # return search node / NULL pointer
*/
struct bst_node *FSYMBOL(bst_search)(struct bst_root *root, void *arg,
		int32 (*cmp)(void *, void *)) {
	struct bst_node **link = &root->node, *parent = NULL;
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

/* @func: bst_erase - bst-tree node erase
* @param1: struct bst_root * # root node
* @param2: struct bst_node * # callback arg
* @param3: int32 (*)(void *, void *) # callback
* @return: struct bst_node * # return erase node / NULL pointer
*/
struct bst_node *FSYMBOL(bst_erase)(struct bst_root *root, void *arg,
		int32 (*cmp)(void *, void *)) {
	struct bst_node **link = &root->node, *tmp = NULL,
		*parent = NULL, *node = NULL, *successor;
	while (*link) {
		tmp = *link;
		int32 m = cmp(tmp, arg);
		if (m < 0) {
			link = &tmp->left;
		} else if (m > 0) {
			link = &tmp->right;
		} else {
			node = tmp;
			break;
		}
		parent = tmp;
	}
	if (!node)
		return NULL;

	/*
	*   n          r          r
	*  / \        / \        / \
	* l   r  --> l   n  --> l   r2
	*    / \        / \
	*  nil  r2    nil  r2
	*
	*    n          l2         l2
	*   / \        / \        / \
	*  l   r  --> l   r  --> l   r
	*     / \        / \        / \
	*   l2  r2      n  r2      r3  r2
	*   / \        / \
	* nil  r3    nil  r3
	*/
	if (node->left && node->right) {
		successor = node->right;
		if (successor->left) {
			do {
				tmp = successor;
				successor = successor->left;
			} while (successor->left);

			tmp->left = successor->right;
			successor->right = node->right;
		}

		successor->left = node->left;
		FSYMBOL(bst_change_child)(root, parent, node, successor);
	} else if (node->left) {
		FSYMBOL(bst_change_child)(root, parent, node, node->left);
	} else if (node->right) {
		FSYMBOL(bst_change_child)(root, parent, node, node->right);
	} else {
		FSYMBOL(bst_change_child)(root, parent, node, NULL);
	}

	return node;
} /* end */
