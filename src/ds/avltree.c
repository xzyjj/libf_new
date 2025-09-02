/* avltree.c - avl self-balancing binary tree implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/avltree.h>


/* @func: avl_change_child - change parent node's child
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # parent node
* @param2: struct avl_node * # old node
* @param2: struct avl_node * # new node
* @return: void
*/
void FSYMBOL(avl_change_child)(struct avl_root *root, struct avl_node *parent,
		struct avl_node *old, struct avl_node *new) {
	if (!parent) {
		root->node = new;
	} else if (parent->left == old) {
		parent->left = new;
	} else {
		parent->right = new;
	}
} /* end */

/* @func: avl_update_height - update node height
* @param1: struct avl_node * # avl-tree node
* @return: void
*/
void FSYMBOL(avl_update_height)(struct avl_node *node) {
	int32 lh = AVL_HEIGHT(node->left);
	int32 rh = AVL_HEIGHT(node->right);

	node->bf = 1 + AVL_MAX(lh, rh);
} /* end */

/* @func: avl_balance_factor - get node balance factor
* @param1: struct avl_node * # avl-tree node
* @return: void
*/
int32 FSYMBOL(avl_balance_factor)(struct avl_node *node) {
	int32 lh = AVL_HEIGHT(node->left);
	int32 rh = AVL_HEIGHT(node->right);

	return lh - rh;
} /* end */

/* @func: avl_left_rotate - avl-tree left rotate
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # rotate node's parent
* @return: void
*      g           g
*     / \         / \
*    p   u  -->  n   u
*   / \         / \
*  s   n       p   r
*     / \     / \
*    l   r   s   l
*/
void FSYMBOL(avl_left_rotate)(struct avl_root *root, struct avl_node *parent) {
	struct avl_node *gparent, *node, *child;
	gparent = parent->parent;
	node = parent->right;
	child = node->left;

	/* 'left node' becomes 'parent right' */
	parent->right = child;
	if (child)
		child->parent = parent;

	/* 'parent' becomes 'left node' */
	node->left = parent;
	parent->parent = node;

	/* 'node' becomes 'gparent child' */
	FSYMBOL(avl_change_child)(root, gparent, parent, node);
	node->parent = gparent;

	/* update height */
	FSYMBOL(avl_update_height)(parent);
	FSYMBOL(avl_update_height)(node);
} /* end */

/* @func: avl_right_rotate - avl-tree right rotate
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # rotate node's parent
* @return: void
*       g         g
*      / \       / \
*     p   u --> n   u
*    / \       / \
*   n   s     l   p
*  / \           / \
* l   r         r   s
*/
void FSYMBOL(avl_right_rotate)(struct avl_root *root, struct avl_node *parent) {
	struct avl_node *gparent, *node, *child;
	gparent = parent->parent;
	node = parent->left;
	child = node->right;

	/* 'right node' becomes 'parent left' */
	parent->left = child;
	if (child)
		child->parent = parent;

	/* 'parent' becomes 'right node' */
	node->right = parent;
	parent->parent = node;

	/* 'node' becomes 'gparent child' */
	FSYMBOL(avl_change_child)(root, gparent, parent, node);
	node->parent = gparent;

	/* update height */
	FSYMBOL(avl_update_height)(parent);
	FSYMBOL(avl_update_height)(node);
} /* end */

/* @func: avl_balance - fix avl-tree balance
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # fix node
* @return: void
*/
void FSYMBOL(avl_balance)(struct avl_root *root, struct avl_node *node) {
	struct avl_node *parent = NULL;
	for (; node; node = parent) {
		FSYMBOL(avl_update_height)(node);

		parent = node->parent;
		if (!parent)
			break;

		int32 bf = FSYMBOL(avl_balance_factor)(parent);
		if (bf > 1) { /* LL LR */
			/* LR */
			if (FSYMBOL(avl_balance_factor)(parent->left) < 0)
				FSYMBOL(avl_left_rotate)(root, parent->left);
			/* LL */
			FSYMBOL(avl_right_rotate)(root, parent);
		} else if (bf < -1) { /* RR RL */
			/* RL */
			if (FSYMBOL(avl_balance_factor)(parent->right) > 0)
				FSYMBOL(avl_right_rotate)(root, parent->right);
			/* RR */
			FSYMBOL(avl_left_rotate)(root, parent);
		}
	}
} /* end */

/* @func: avl_erase_fix - fix avl-tree erase balance
* @param1: struct avl_root * # root node
* @param2: struct avl_node * # erase node
* @return: void
*/
void FSYMBOL(avl_erase_fix)(struct avl_root *root, struct avl_node *node) {
	struct avl_node *parent = node->parent, *left = node->left,
		*right = node->right, *successor;
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
	if (left && right) {
		successor = right;
		if (successor->left) {
			do {
				parent = successor;
				successor = successor->left;
			} while (successor->left);

			/* 'successor right' becomes 'parent left' */
			parent->left = successor->right;
			if (parent->left)
				parent->left->parent = parent;

			/* 'node right' becomes 'successor right' */
			successor->right = right;
			right->parent = successor;
		}

		/* 'node left' becomes 'successor left' */
		successor->left = left;
		left->parent = successor;

		/* 'successor' becomes 'node parent child' */
		FSYMBOL(avl_change_child)(root, node->parent, node, successor);
		successor->parent = node->parent;

		if (successor == right) {
			FSYMBOL(avl_balance)(root, left);
		} else {
			FSYMBOL(avl_balance)(root, successor);
		}

		return;
	}
	/*
	*     p         p
	*    / \       / \
	*   n   s --> l   s
	*  / \
	* l  nil
	*/
	if (left) {
		/* 'node left' becomes 'parent child' */
		FSYMBOL(avl_change_child)(root, parent, node, left);
		left->parent = parent;

		FSYMBOL(avl_balance)(root, left);

		return;
	}
	/*
	*      p         p
	*     / \       / \
	*    n   s --> r   s
	*   / \
	* nil  r
	*/
	if (right) {
		/* 'node right' becomes 'parent child' */
		FSYMBOL(avl_change_child)(root, parent, node, right);
		right->parent = parent;

		FSYMBOL(avl_balance)(root, right);

		return;
	}

	/*
	*      p           p
	*     / \         / \
	*    n   s -->  nil  s
	*   / \
	* nil nil
	*/
	FSYMBOL(avl_change_child)(root, parent, node, NULL);
	if (parent) {
		/* sibling node */
		if (parent->left) {
			FSYMBOL(avl_balance)(root, parent->left);
		} else if (parent->right) {
			FSYMBOL(avl_balance)(root, parent->right);
		} else { /* parent */
			FSYMBOL(avl_balance)(root, parent);
		}
	} /* 'parent' is nil == root */
} /* end */
