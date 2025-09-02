#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/ds/bits_get.h>


int main(void) {
	uint8 *s = (uint8 *)"Hell";
	BITS_GET_NEW(ctx, s, 4);

	uint32 v = 0, v1 = 0, v2;

	FSYMBOL(bits_get)(&ctx, &v, 8, 0);
	printf("H: %c\n", v);

	FSYMBOL(bits_get)(&ctx, &v, 4, 0);
	FSYMBOL(bits_get)(&ctx, &v1, 3, 0);
	FSYMBOL(bits_get)(&ctx, &v2, 9, 0);

	v |= (v1 << 4) | (v2 << 7);

	printf("el: %c%c\n", v & 0xff, v >> 8);

	return 0;
}
