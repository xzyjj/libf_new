/* xml_tree.h - xml data object tree definitions */

#ifndef LIBF_UL_XML_TREE_H
#define LIBF_UL_XML_TREE_H

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ul/xml_parser.h>
#include <libf/ds/list.h>


/* @def: xml_tree */
#undef XML_TREE_FG_ELEMENT
#define XML_TREE_FG_ELEMENT 0x01
#undef XML_TREE_FG_EMPTY_END
#define XML_TREE_FG_EMPTY_END 0x02
#undef XML_TREE_FG_EMBED
#define XML_TREE_FG_EMBED 0x04
#undef XML_TREE_FG_DOCTYPE
#define XML_TREE_FG_DOCTYPE 0x08
#undef XML_TREE_FG_STATEMENT
#define XML_TREE_FG_STATEMENT 0x10
#undef XML_TREE_FG_COMMENT
#define XML_TREE_FG_COMMENT 0x20
#undef XML_TREE_FG_CDATA
#define XML_TREE_FG_CDATA 0x40

#undef XML_TREE_TYPE_STRDUP
#define XML_TREE_TYPE_STRDUP 1
#undef XML_TREE_TYPE_ATTR
#define XML_TREE_TYPE_ATTR 2
#undef XML_TREE_TYPE_NODE
#define XML_TREE_TYPE_NODE 3
#undef XML_TREE_TYPE_STACK
#define XML_TREE_TYPE_STACK 4

#undef xml_tree_attr
struct xml_tree_attr {
	struct list list;
	char *name;
	char *val;
};

#undef xml_tree_node
struct xml_tree_node {
	struct list list;
	struct list_head embed;
	struct list_head attr;
	char *name;
	char *str;
	char *str_e;
	int32 flags;
};

#undef xml_tree_stack
struct xml_tree_stack {
	struct list list;
	struct xml_tree_node *tmp_node;
	struct xml_tree_attr *tmp_attr;
	struct xml_tree_node *tmp_embed;
};

#undef xml_tree_root
struct xml_tree_root {
	struct list_head head;
	struct list_head stack;
	struct xml_ctx xml_ctx;
	void *arg;
	/* type, length, arg */
	void *(*call_alloc)(int32, int32, void *);
	/* type, pointer, arg */
	int32 (*call_free)(int32, void *, void *);
};

#undef XML_TREE_NEW
#define XML_TREE_NEW(name, _alloc, _free, _arg) \
	struct xml_tree_root name = { \
		.call_alloc = _alloc, \
		.call_free = _free, \
		.arg = _arg \
		}

#undef XML_TREE_INIT
#define XML_TREE_INIT(x, _alloc, _free, _arg) \
	(x)->call_alloc = _alloc; \
	(x)->call_free = _free; \
	(x)->arg = _arg
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* xml_tree.c */
extern int32 FSYMBOL(xml_tree_free)(struct xml_tree_root *root);
extern int32 FSYMBOL(xml_tree_parser)(struct xml_tree_root *root,
		const char *s);

#ifdef __cplusplus
}
#endif


#endif
