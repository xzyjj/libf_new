#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xfloat.h>
#include <libf/ul/xml_parser.h>
#include <libf/ds/list.h>


struct TL {
	struct list node;
	const char *p;
};

struct T {
	struct list_head head;
	int32 lev;
};

static int32 _call(int32 type, const char *s, int32 len, void *arg) {
#if 1

	struct T *t = arg;
	struct TL *tl;

	switch (type) {
		case XML_ELEMENT_START:
			printf("%*s'", t->lev, "<");
			fwrite(s, 1, len, stdout);
			printf("'>\n");

			tl = malloc(sizeof(struct TL));
			tl->p = s;
			FSYMBOL(list_add)(&t->head, &tl->node);
			t->lev++;

			break;
		case XML_ELEMENT_ATTR_NAME:
			printf("%*s", t->lev, "");
			printf(" ");
			fwrite(s, 1, len, stdout);
			printf("=");
			break;
		case XML_ELEMENT_ATTR_VALUE:
			printf("\"");
			fwrite(s, 1, len, stdout);
			printf("\"\n");
			break;
		case XML_ELEMENT_STRING:
			printf("%*s", t->lev, "");
			fwrite(s, 1, len, stdout);
			printf("\n");
			break;
		case XML_DOCTYPE_START:
			printf("<!DOCTYPE>\n");
			break;
		case XML_DOCTYPE_NAME:
			printf("%*s ", t->lev, "");
			fwrite(s, 1, len, stdout);
			printf("\n");
			break;
		case XML_DOCTYPE_STRING:
			printf("%*s ", t->lev, "");
			printf("\"");
			fwrite(s, 1, len, stdout);
			printf("\"\n");
			break;
		case XML_STATEMENT_START:
			printf("%*s<?'", t->lev, "");
			fwrite(s, 1, len, stdout);
			printf("'?>\n");
			break;
		case XML_STATEMENT_ATTR_NAME:
			printf("%*s", t->lev, "");
			printf(" ");
			fwrite(s, 1, len, stdout);
			printf("=");
			break;
		case XML_STATEMENT_ATTR_VALUE:
			printf("\"");
			fwrite(s, 1, len, stdout);
			printf("\"\n");
			break;
		case XML_COMMENT:
			printf("%*s<!--", t->lev, "");
			fwrite(s, 1, len, stdout);
			printf("-->\n");
			break;
		case XML_CDATA:
			printf("%*s<![CDATA[", t->lev, "");
			fwrite(s, 1, len, stdout);
			printf("]]>\n");
			break;
		default:
			return -1;
	}

#endif

	return 0;
}

static int32 _call_end(int32 type, const char *s, int32 len, void *arg) {
#if 1

	struct T *t = arg;
	struct TL *tl = container_of(t->head.node, struct TL, node);
	t->lev--;

	switch (type) {
		case XML_ELEMENT_END:
			printf("%*s/'", t->lev, "<");
			fwrite(s, 1, len, stdout);
			printf("'>\n");

			if (strncmp(tl->p, s, len))
				return -1;
			FSYMBOL(list_del)(&t->head, &tl->node);
			free(tl);

			break;
		case XML_ELEMENT_EMPTY_END:
			FSYMBOL(list_del)(&t->head, &tl->node);
			free(tl);

			printf("/\n");
			break;
		default:
			return -1;
	}

#endif

	return 0;
}

int main(void) {
	clock_t start, end;
	float64 time;
	char *s = malloc(4 << 20);
	memset(s, 0, 4 << 20);
	fread(s, 1, ((4 << 20) - 1), stdin);

	struct T t = { .head.node = NULL, 1 };
	XML_NEW(ctx, _call, _call_end, &t);

	start = clock();
	int32 ret = FSYMBOL(xml_parser)(&ctx, s);
	end = clock();
	time = (float64)(end - start) / CLOCKS_PER_SEC;

	printf("\nret: %d, len: %d, err: %d, time: %.6f\n",
		ret, ctx.len, ctx.err, time);

	return 0;
}
