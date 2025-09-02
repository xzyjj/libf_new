#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bits_add.h>


int main(void) {
	uint8 *s = (uint8 *)"Hell";
	BITS_ADD_NEW(ctx);

	FSYMBOL(bits_add)(&ctx, s[0], 8);
	FSYMBOL(bits_add)(&ctx, s[1], 3);
	FSYMBOL(bits_add)(&ctx, s[1] >> 3, 4);
	FSYMBOL(bits_add)(&ctx, s[1] >> 7, 1);
	FSYMBOL(bits_add)(&ctx, s[2], 8);

	FSYMBOL(bits_add)(&ctx, 0, 8);

	printf("Hel: %s\n", BITS_ADD_BUF(&ctx));

	return 0;
}
