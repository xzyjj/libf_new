/* bsttree.h - binary search tree definitions */

#ifndef LIBF_DS_BSTTREE_H
#define LIBF_DS_BSTTREE_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: bsttree */
#undef bst_node
struct bst_node {
	struct bst_node *left, *right;
};

#undef bst_root
struct bst_root {
	struct bst_node *node;
};

#undef BST_ROOT_NEW
#define BST_ROOT_NEW(x) struct bst_root x = { NULL }
#undef BST_ROOT_INIT
#define BST_ROOT_INIT(x) (x)->node = NULL

#undef BST_LEFT
#define BST_LEFT (-1)
#undef BST_RIGHT
#define BST_RIGHT 1
#undef BST_REPEAT
#define BST_REPEAT 0
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* bsttree.c */
extern void FSYMBOL(bst_change_child)(struct bst_root *root, struct bst_node *parent,
		struct bst_node *old, struct bst_node *_new);
extern struct bst_node *FSYMBOL(bst_insert)(struct bst_root *root,
		struct bst_node *_new, int32 (*cmp)(void *, void *));
extern struct bst_node *FSYMBOL(bst_search)(struct bst_root *root, void *arg,
		int32 (*cmp)(void *, void *));
extern struct bst_node *FSYMBOL(bst_erase)(struct bst_root *root, void *arg,
		int32 (*cmp)(void *, void *));

#ifdef __cplusplus
}
#endif


#endif
