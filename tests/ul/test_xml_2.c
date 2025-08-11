#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstdlib_sys.h>
#include <libf/ul/xml_tree.h>
#include <libf/ds/list.h>


#define malloc xmalloc
#define free xfree

static void *_alloc(int32 type, int32 len, void *arg) {
	(void)arg;

	switch (type) {
		case XML_TREE_TYPE_STRDUP:
			return malloc(len);
		case XML_TREE_TYPE_ATTR:
			return malloc(sizeof(struct xml_tree_attr));
		case XML_TREE_TYPE_NODE:
			return malloc(sizeof(struct xml_tree_node));
		case XML_TREE_TYPE_STACK:
			return malloc(sizeof(struct xml_tree_stack));
		default:
			return NULL;
	}

	return NULL;
}

static int32 _free(int32 type, void *p, void *arg) {
	(void)arg;

	switch (type) {
		case XML_TREE_TYPE_STRDUP:
		case XML_TREE_TYPE_ATTR:
		case XML_TREE_TYPE_NODE:
		case XML_TREE_TYPE_STACK:
			free(p);
			break;
		default:
			return -1;
	}

	return 0;
}

static void xml_tree_element_print(struct list *node) {
	struct xml_tree_node *tmp = container_of(node,
		struct xml_tree_node, list);

	printf("<%s", tmp->name);

	struct xml_tree_attr *attr;
	LIST_FOR_EACH(tmp->attr.node, pos) {
		attr = container_of(pos, struct xml_tree_attr, list);
		printf(" %s=\"%s\"", attr->name,
			attr->val ? attr->val : "");
	}

	if (tmp->flags & XML_TREE_FG_EMPTY_END) {
		printf(" />");
	} else {
		printf(">");
	}

	if (tmp->str)
		printf("%s", tmp->str);
}

static void xml_tree_doctype_print(struct list *node) {
	printf("<!DOCTYPE");

	struct xml_tree_attr *attr;
	LIST_FOR_EACH(node, pos) {
		attr = container_of(pos, struct xml_tree_attr, list);
		if (attr->name) {
			printf(" %s", attr->name);
		} else {
			printf(" \"%s\"", attr->val);
		}
	}

	printf(">");
}

static void xml_tree_statement_print(struct list *node) {
	struct xml_tree_node *tmp = container_of(node,
		struct xml_tree_node, list);

	printf("<?%s", tmp->name ? tmp->name : "");

	struct xml_tree_attr *attr;
	LIST_FOR_EACH(tmp->attr.node, pos) {
		attr = container_of(pos, struct xml_tree_attr, list);
		printf(" %s=\"%s\"", attr->name,
			attr->val ? attr->val : "");
	}

	printf("?>");
}

static void xml_tree_print(struct list *node, int32 n) {
	struct xml_tree_node *tmp;

	LIST_FOR_EACH(node, pos) {
		tmp = container_of(pos, struct xml_tree_node, list);

		if (tmp->flags & XML_TREE_FG_COMMENT) {
			if (tmp->str_e)
				printf("%s", tmp->str_e);
			continue;
		}

		if (tmp->flags & XML_TREE_FG_CDATA) {
			printf("<![CDATA[%s]]>", tmp->str);
			if (tmp->str_e)
				printf("%s", tmp->str_e);
			continue;
		}

		if (tmp->flags & XML_TREE_FG_STATEMENT) {
			xml_tree_statement_print(pos);
			if (!n)
				printf("\n");
			continue;
		}

		if (tmp->flags & XML_TREE_FG_DOCTYPE) {
			xml_tree_doctype_print(tmp->attr.node);
			if (!n)
				printf("\n");
			continue;
		}


		if (tmp->flags & XML_TREE_FG_ELEMENT) {
			xml_tree_element_print(pos);
			if (tmp->flags & XML_TREE_FG_EMPTY_END) {
				if (tmp->str_e)
					printf("%s", tmp->str_e);
				if (!n)
					printf("\n");
				continue;
			}
		}

		if (tmp->flags & XML_TREE_FG_EMBED)
			xml_tree_print(tmp->embed.node, n + 1);

		if (tmp->flags & XML_TREE_FG_ELEMENT) {
			printf("</%s>", tmp->name);
			if (tmp->str_e)
				printf("%s", tmp->str_e);
			if (!n)
				printf("\n");
		}
	}

	if (!n)
		printf("\n");
}

int main(void) {
	clock_t start, end;
	double time;
	char *s = malloc(4 << 20);
	memset(s, 0, 4 << 20);
	fread(s, 1, ((4 << 20) - 1), stdin);

	XML_TREE_NEW(root, _alloc, _free, NULL);

	start = clock();
	int32 ret = FSYMBOL(xml_tree_parser)(&root, s);
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;

	xml_tree_print(root.head.node, 0);
	FSYMBOL(xml_tree_free)(&root);

	fprintf(stderr, "\nret: %d, len: %d, err: %d, time: %.6f\n",
		ret, root.xml_ctx.len, root.xml_ctx.err, time);

	return 0;
}
