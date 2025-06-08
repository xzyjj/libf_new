#include <stdio.h>
#include <stdlib.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/ul/ini_parser.h>


static int32 _call(int32 type, const char *s, int32 len, void *arg) {
	switch (type) {
		case INI_SECTION_TYPE:
			printf("[");
			fwrite(s, 1, len, stdout);
			printf("]\n");
			break;
		case INI_KEY_TYPE:
			fwrite(s, 1, len, stdout);
			printf(" = ");
			break;
		case INI_VALUE_TYPE:
			fwrite(s, 1, len, stdout);
			printf("\n");
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

	INI_NEW(ctx, _call, NULL);
	int32 ret = FSYMBOL(ini_parser)(&ctx, s);

	printf("\nret: %d, line: %d, err: %d\n", ret, ctx.line, ctx.err);

	return 0;
}
