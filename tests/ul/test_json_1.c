#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libf/sl/xstdint.h>
#include <libf/ul/json_parser.h>


static int32 _call(int32 type, const char *s, int32 len, void *arg) {
	switch (type) {
		case JSON_ARRAY_TYPE:
			printf("%*s\n", *((int32 *)arg), "[");
			(*((int32 *)arg))++;
			break;
		case JSON_OBJECT_TYPE:
			printf("%*s\n", *((int32 *)arg), "{");
			(*((int32 *)arg))++;
			break;
		case JSON_VALUE_TYPE:
			printf("%*s%s", *((int32 *)arg), "", "\"");
			fwrite(s, 1, len, stdout);
			printf("\": ");
			break;
		case JSON_STRING_TYPE:
			printf("%*s%s", *((int32 *)arg), "", "\"");
			fwrite(s, 1, len, stdout);
			printf("\"\n");
			break;
		case JSON_NUMBER_TYPE:
			printf("%*s", *((int32 *)arg), "");
			fwrite(s, 1, len, stdout);
			printf("\n");
			break;
		case JSON_NULL_TYPE:
			printf("%*s%s\n", *((int32 *)arg), "", "Null");
			break;
		case JSON_TRUE_TYPE:
			printf("%*s%s\n", *((int32 *)arg), "", "True");
			break;
		case JSON_FALSE_TYPE:
			printf("%*s%s\n", *((int32 *)arg), "", "False");
			break;
		default:
			return -1;
	}

	return 0;
}

static int32 _call_end(int32 type, void *arg) {
	(*((int32 *)arg))--;
	switch (type) {
		case JSON_ARRAY_TYPE:
			printf("%*s\n", *((int32 *)arg), "]");
			break;
		case JSON_OBJECT_TYPE:
			printf("%*s\n", *((int32 *)arg), "}");
			break;
		default:
			return -1;
	}

	return 0;
}

int main(void) {
	char *s = malloc(4 << 20);
	memset(s, 0, 4 << 20);
	fread(s, 1, ((4 << 20) - 1), stdin);

	int32 lev = 1;
	JSON_NEW(ctx, _call, _call_end, &lev);
	int32 ret = FSYMBOL(json_parser)(&ctx, s);

	printf("\nret: %d, len: %d, err: %d\n", ret, ctx.len, ctx.err);

	return 0;
} 
