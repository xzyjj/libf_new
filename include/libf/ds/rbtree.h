/* rbtree.h - red-black tree definitions */

#ifndef LIBF_DS_RBTREE_H
#define LIBF_DS_RBTREE_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>


/* @def: rbtree */
struct rb_node {
	int32 color;
	struct rb_node *parent, *left, *right;
};

struct rb_root {
	struct rb_node *node;
};

#undef RB_ROOT_NEW
#define RB_ROOT_NEW(x) struct rb_root x = { NULL }
#undef RB_ROOT_INIT
#define RB_ROOT_INIT(x) ((x)->node = NULL)

#undef RB_RED
#define RB_RED 0
#undef RB_BLACK
#define RB_BLACK 1
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* rbtree.c */
extern void FSYMBOL(rb_change_child)(struct rb_root *root, struct rb_node *parent,
		struct rb_node *old, struct rb_node *_new);
extern void FSYMBOL(rb_left_rotate)(struct rb_root *root, struct rb_node *parent);
extern void FSYMBOL(rb_right_rotate)(struct rb_root *root, struct rb_node *parent);
extern void FSYMBOL(rb_insert_fix)(struct rb_root *root, struct rb_node *node);
extern void FSYMBOL(rb_erase_fix)(struct rb_root *root, struct rb_node *node);

/* rbtree_wrap.c */
extern struct rb_node *FSYMBOL(rb_insert_wrap)(struct rb_root *root,
		struct rb_node *_new, int32 (*cmp)(void *, void *));
extern struct rb_node *FSYMBOL(rb_search_wrap)(struct rb_root *root,
		void *arg, int32 (*cmp)(void *, void *));
extern struct rb_node *FSYMBOL(rb_erase_wrap)(struct rb_root *root,
		struct rb_node *old);
extern struct rb_node *FSYMBOL(rb_erase2_wrap)(struct rb_root *root,
		void *arg, int32 (*cmp)(void *, void *));

#ifdef __cplusplus
}
#endif


#endif
