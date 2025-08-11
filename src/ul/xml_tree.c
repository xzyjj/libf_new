/* xml_tree.c - xml data object tree implementations */

#include <libf/config.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/ul/xml_parser.h>
#include <libf/ul/xml_tree.h>
#include <libf/ds/list.h>


/* @def: xml_tree
* head | node <--> node <--> node --> end
*        node  | { list, name, attr, str, str_e, flags, embed }
*        embed | node <--> node <--> node --> end
*                node ...
*                embed...
*                str_e<parent node middle string>
*/
/* end */

/* @func: _strndup (static) - xml-tree duplicate a string
* @param1: struct xml_tree_root * # xml-tree root node
* @return: char *                 # string pointer
*/
static char *_strndup(struct xml_tree_root *root, const char *s,
		uint32 n) {
	char *p = root->call_alloc(XML_TREE_TYPE_STRDUP,
		n + 1,
		root->arg);
	if (!p)
		return NULL;

	XSYMBOL(memcpy)(p, s, n);
	p[n] = '\0';

	return p;
} /* end */

/* @func: _free_strndup (static) - xml-tree free duplicate a string
* @param1: struct xml_tree_root * # xml-tree root node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _free_strndup(struct xml_tree_root *root, void *p) {
	return root->call_free(XML_TREE_TYPE_STRDUP, p, root->arg);
} /* end */

/* @func: _del_tree_attr (static) - xml-tree new attr node
* @param1: struct xml_tree_root * # xml-tree root node
* @return: void *                 # allocated attr node
*/
static void *_new_tree_attr(struct xml_tree_root *root) {
	void *p = root->call_alloc(XML_TREE_TYPE_ATTR,
		sizeof(struct xml_tree_attr),
		root->arg);
	if (!p)
		return NULL;

	return XSYMBOL(memset)(p, 0, sizeof(struct xml_tree_attr));
} /* end */

/* @func: _del_tree_node (static) - xml-tree new node
* @param1: struct xml_tree_root * # xml-tree root node
* @return: void *                 # allocated node
*/
static void *_new_tree_node(struct xml_tree_root *root) {
	void *p = root->call_alloc(XML_TREE_TYPE_NODE,
		sizeof(struct xml_tree_node),
		root->arg);
	if (!p)
		return NULL;

	return XSYMBOL(memset)(p, 0, sizeof(struct xml_tree_node));
} /* end */

/* @func: _del_tree_stack (static) - xml-tree new stack node
* @param1: struct xml_tree_root * # xml-tree root node
* @return: void *                 # allocated stack node
*/
static void *_new_tree_stack(struct xml_tree_root *root) {
	void *p = root->call_alloc(XML_TREE_TYPE_STACK,
		sizeof(struct xml_tree_stack),
		root->arg);
	if (!p)
		return NULL;

	return XSYMBOL(memset)(p, 0, sizeof(struct xml_tree_stack));
} /* end */

/* @func: _del_tree_attr (static) - xml-tree free attr node
* @param1: struct xml_tree_root * # xml-tree root node
* @param2: void *                 # xml-tree attr node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _del_tree_attr(struct xml_tree_root *root, void *p) {
	return root->call_free(XML_TREE_TYPE_ATTR, p, root->arg);
} /* end */

/* @func: _del_tree_node (static) - xml-tree free node
* @param1: struct xml_tree_root * # xml-tree root node
* @param2: void *                 # xml-tree node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _del_tree_node(struct xml_tree_root *root, void *p) {
	return root->call_free(XML_TREE_TYPE_NODE, p, root->arg);
} /* end */

/* @func: _del_tree_stack (static) - xml-tree free stack node
* @param1: struct xml_tree_root * # xml-tree root node
* @param2: void *                 # xml-tree stack node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _del_tree_stack(struct xml_tree_root *root, void *p) {
	return root->call_free(XML_TREE_TYPE_STACK, p, root->arg);
} /* end */

/* @func: _call (static) - xml-tree parser callback begin
* @param1: int32        # xml type
* @param2: const char * # string
* @param3: int32        # length
* @param4: void *       # argument
* @return: int32        # 0: no error, -1: error
*/
static int32 _call(int32 type, const char *s, int32 len, void *arg) {
	struct xml_tree_root *root = arg;
	struct xml_tree_attr *attr;
	struct xml_tree_node *node, *tmp_node;
	struct xml_tree_stack *stack;
	struct list_head *head;

	switch (type) {
		case XML_ELEMENT_START:
			node = _new_tree_node(root); /* new */
			if (!node)
				return -1;
			node->flags = XML_TREE_FG_ELEMENT;

			/* strdup */
			node->name = _strndup(root, s, len);
			if (!node->name)
				return -1;

			if (root->stack.node) {
				stack = container_of(root->stack.node,
					struct xml_tree_stack, list);
				stack->tmp_embed = node;

				tmp_node = stack->tmp_node;
				tmp_node->flags |= XML_TREE_FG_EMBED;
				head = &tmp_node->embed;
			} else {
				head = &root->head;
			}

			stack = _new_tree_stack(root); /* new */
			if (!stack)
				return -1;
			stack->tmp_node = node;
			FSYMBOL(list_add)(&root->stack, &stack->list);

			FSYMBOL(list_add_tail)(head, &node->list);
			break;
		case XML_ELEMENT_ATTR_NAME:
			attr = _new_tree_attr(root);
			if (!attr)
				return -1;

			/* strdup */
			attr->name = _strndup(root, s, len);
			if (!attr->name)
				return -1;

			stack = container_of(root->stack.node,
				struct xml_tree_stack, list);
			stack->tmp_attr = attr;

			node = stack->tmp_node;
			FSYMBOL(list_add_tail)(&node->attr, &attr->list);
			break;
		case XML_ELEMENT_ATTR_VALUE:
			if (!len)
				break;

			stack = container_of(root->stack.node,
				struct xml_tree_stack, list);

			attr = stack->tmp_attr;
			/* strdup */
			attr->val = _strndup(root, s, len);
			if (!attr->val)
				return -1;
			break;
		case XML_ELEMENT_STRING:
			if (!len)
				break;

			stack = container_of(root->stack.node,
				struct xml_tree_stack, list);

			if (stack->tmp_embed) { /* prev a element */
				node = stack->tmp_embed;
				stack->tmp_embed = NULL;
				/* strdup */
				node->str_e = _strndup(root, s, len);
				if (!node->str_e)
					return -1;
			} else {
				node = stack->tmp_node;
				/* strdup */
				node->str = _strndup(root, s, len);
				if (!node->str)
					return -1;
			}
			break;
		case XML_DOCTYPE_START:
			node = _new_tree_node(root); /* new */
			if (!node)
				return -1;
			node->flags = XML_TREE_FG_DOCTYPE;

			stack = _new_tree_stack(root); /* new */
			if (!stack)
				return -1;
			stack->tmp_node = node;
			FSYMBOL(list_add)(&root->stack, &stack->list);

			FSYMBOL(list_add_tail)(&root->head, &node->list);
			break;
		case XML_DOCTYPE_NAME:
			attr = _new_tree_attr(root); /* new */
			if (!attr)
				return -1;

			/* strdup */
			attr->name = _strndup(root, s, len);
			if (!attr->name)
				return -1;

			stack = container_of(root->stack.node,
					struct xml_tree_stack, list);

			node = stack->tmp_node;
			FSYMBOL(list_add_tail)(&node->attr, &attr->list);
			break;
		case XML_DOCTYPE_STRING:
			if (!len)
				break;

			attr = _new_tree_attr(root); /* new */
			if (!attr)
				return -1;

			/* strdup */
			attr->val = _strndup(root, s, len);
			if (!attr->val)
				return -1;

			stack = container_of(root->stack.node,
				struct xml_tree_stack, list);

			node = stack->tmp_node;
			FSYMBOL(list_add_tail)(&node->attr, &attr->list);
			break;
		case XML_STATEMENT_START:
			node = _new_tree_node(root); /* new */
			if (!node)
				return -1;
			node->flags = XML_TREE_FG_STATEMENT;

			/* strdup */
			node->name = _strndup(root, s, len);
			if (!node->name)
				return -1;

			stack = _new_tree_stack(root); /* new */
			if (!stack)
				return -1;
			stack->tmp_node = node;
			FSYMBOL(list_add)(&root->stack, &stack->list);

			FSYMBOL(list_add_tail)(&root->head, &node->list);
			break;
		case XML_STATEMENT_ATTR_NAME:
			attr = _new_tree_attr(root); /* new */
			if (!attr)
				return -1;

			/* strdup */
			attr->name = _strndup(root, s, len);
			if (!attr->name)
				return -1;

			stack = container_of(root->stack.node,
					struct xml_tree_stack, list);
			stack->tmp_attr = attr;

			node = stack->tmp_node;
			FSYMBOL(list_add_tail)(&node->attr, &attr->list);
			break;
		case XML_STATEMENT_ATTR_VALUE:
			if (!len)
				break;

			stack = container_of(root->stack.node,
				struct xml_tree_stack, list);

			attr = stack->tmp_attr;
			/* strdup */
			attr->val = _strndup(root, s, len);
			if (!attr->val)
				return -1;
			break;
		case XML_COMMENT:
			if (!len)
				break;

			node = _new_tree_node(root); /* new */
			if (!node)
				return -1;
			node->flags = XML_TREE_FG_COMMENT;

			/* strdup */
			node->str = _strndup(root, s, len);
			if (!node->str)
				return -1;

			if (root->stack.node) {
				stack = container_of(root->stack.node,
					struct xml_tree_stack, list);
				stack->tmp_embed = node;

				tmp_node = stack->tmp_node;
				tmp_node->flags |= XML_TREE_FG_EMBED;
				head = &tmp_node->embed;
			} else {
				head = &root->head;
			}

			FSYMBOL(list_add_tail)(head, &node->list);
			break;
		case XML_CDATA:
			if (!len)
				break;

			node = _new_tree_node(root); /* new */
			if (!node)
				return -1;
			node->flags = XML_TREE_FG_CDATA;

			/* strdup */
			node->str = _strndup(root, s, len);
			if (!node->str)
				return -1;

			stack = container_of(root->stack.node,
				struct xml_tree_stack, list);
			stack->tmp_embed = node;

			tmp_node = stack->tmp_node;
			tmp_node->flags |= XML_TREE_FG_EMBED;
			head = &tmp_node->embed;

			FSYMBOL(list_add_tail)(head, &node->list);
			break;
		default:
			return -1;
	}

	return 0;
} /* end */

/* @func: _call_end (static) - xml-tree parser callback end
* @param1: int32        # xml type
* @param2: const char * # string
* @param3: int32        # length
* @param4: void *       # argument
* @return: int32        # 0: no error, -1: error
*/
static int32 _call_end(int32 type, const char *s, int32 len, void *arg) {
	struct xml_tree_root *root = arg;
	struct xml_tree_node *node;
	struct xml_tree_stack *stack;

	stack = container_of(root->stack.node,
		struct xml_tree_stack, list);
	node = stack->tmp_node;

	switch (type) {
		case XML_ELEMENT_END:
			if (XSYMBOL(strlen)(node->name) != (uint32)len)
				return -1;
			if (XSYMBOL(strncmp)(node->name, s, len))
				return -1;

			FSYMBOL(list_del)(&root->stack, &stack->list);
			if (_del_tree_stack(root, stack)) /* del */
				return -1;
			break;
		case XML_ELEMENT_EMPTY_END:
			node->flags |= XML_TREE_FG_EMPTY_END;
		case XML_DOCTYPE_END:
		case XML_STATEMENT_END:
			FSYMBOL(list_del)(&root->stack, &stack->list);
			if (_del_tree_stack(root, stack)) /* del */
				return -1;
			break;
		default:
			return -1;
	}

	return 0;
} /* end */

/* @func: _xml_tree_free_attr (static) - xml-tree free all attr nodes
* @param1: struct xml_tree_root * # xml-tree root node
* @param2: struct xml_tree_node * # xml-tree node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _xml_tree_free_attr(struct xml_tree_root *root,
		struct xml_tree_node *node) {
	struct xml_tree_attr *attr;
	while (node->attr.node) {
		LIST_FOR_EACH(node->attr.node, pos) {
			attr = container_of(pos, struct xml_tree_attr, list);

			if (attr->name) {
				if (_free_strndup(root, attr->name))
					return -1;
			}
			if (attr->val) {
				if (_free_strndup(root, attr->val))
					return -1;
			}

			FSYMBOL(list_del)(&node->attr, &attr->list);
			if (_del_tree_attr(root, attr)) /* del */
				return -1;
			break;
		}
	}

	return 0;
} /* end */

/* @func: _xml_tree_free_embed (static) - xml-tree free all embedded nodes
* @param1: struct xml_tree_root * # xml-tree root node
* @param2: struct xml_tree_node * # xml-tree embed node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _xml_tree_free_embed(struct xml_tree_root *root,
		struct xml_tree_node *node) {
	struct xml_tree_node *tmp;
	while (node->embed.node) {
		LIST_FOR_EACH(node->embed.node, pos) {
			tmp = container_of(pos, struct xml_tree_node, list);

			if (tmp->flags & XML_TREE_FG_EMBED) {
				if (_xml_tree_free_embed(root, tmp))
					return -1;
			}
			if (tmp->name) {
				if (_free_strndup(root, tmp->name))
					return -1;
			}
			if (tmp->str) {
				if (_free_strndup(root, tmp->str))
					return -1;
			}
			if (tmp->str_e) {
				if (_free_strndup(root, tmp->str_e))
					return -1;
			}
			if (_xml_tree_free_attr(root, tmp))
				return -1;

			FSYMBOL(list_del)(&node->embed, &tmp->list);
			if (_del_tree_node(root, tmp)) /* del */
				return -1;
			break;
		}
	}

	return 0;
} /* end */

/* @func: xml_tree_free - xml-tree free all nodes
* @param1: struct xml_tree_root * # xml-tree root node
* @return: int32                  # 0: no error, -1: error
*/
int32 FSYMBOL(xml_tree_free)(struct xml_tree_root *root) {
	struct xml_tree_node *tmp;
	while (root->head.node) {
		LIST_FOR_EACH(root->head.node, pos) {
			tmp = container_of(pos, struct xml_tree_node, list);

			if (tmp->flags & XML_TREE_FG_ELEMENT) {
				if (_xml_tree_free_embed(root, tmp))
					return -1;
			}
			if (tmp->name) {
				if (_free_strndup(root, tmp->name))
					return -1;
			}
			if (tmp->str) {
				if (_free_strndup(root, tmp->str))
					return -1;
			}
			if (_xml_tree_free_attr(root, tmp))
				return -1;

			FSYMBOL(list_del)(&root->head, &tmp->list);
			if (_del_tree_node(root, tmp)) /* del */
				return -1;
			break;
		}
	}

	return 0;
} /* end */

/* @func: _xml_tree_free_stack (static) - xml-tree free all stack nodes
* @param1: struct xml_tree_root * # xml-tree root node
* @return: int32                  # 0: no error, -1: error
*/
static int32 _xml_tree_free_stack(struct xml_tree_root *root) {
	struct xml_tree_stack *tmp;
	while (root->stack.node) {
		LIST_FOR_EACH(root->stack.node, pos) {
			tmp = container_of(pos, struct xml_tree_stack, list);
			FSYMBOL(list_del)(&root->stack, &tmp->list);
			if (_del_tree_stack(root, tmp)) /* del */
				return -1;
			break;
		}
	}

	return 0;
} /* end */

/* @func: xml_tree_parser - xml-tree parser
* @param1: struct xml_tree_root * # xml-tree root node
* @param2: struct xml_tree_node * # xml-tree node
* @return: int32                  # 0: no error, -1: error
*/
int32 FSYMBOL(xml_tree_parser)(struct xml_tree_root *root,
		const char *s) {
	XML_INIT(&root->xml_ctx, _call, _call_end, root);

	int32 ret = FSYMBOL(xml_parser)(&root->xml_ctx, s);
	_xml_tree_free_stack(root);

	return ret;
} /* end */
