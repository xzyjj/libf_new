#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/poly1305.h>


int main(void) {
	uint8 key[POLY1305_KEYLEN] = {
		0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
		0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8
		};
	uint8 ran[POLY1305_RANLEN] = {
		0xa8, 0x06, 0x1d, 0xc1, 0x30, 0x51, 0x36, 0xc6,
		0xc2, 0x2b, 0x8b, 0xaf, 0x0c, 0x01, 0x27, 0xa9
		};
	char *str = "Cryptographic Forum Research Group";

	POLY1305_NEW(ctx);
	FSYMBOL(poly1305_init)(&ctx, key, ran);

/*	FSYMBOL(poly1305_process)(&ctx, (uint8 *)str, XSYMBOL(strlen)(str));
	FSYMBOL(poly1305_finish)(&ctx);
*/
	FSYMBOL(poly1305)(&ctx, (uint8 *)str, XSYMBOL(strlen)(str));

	for (int32 i = 0; i < POLY1305_TAGLEN; i++)
		printf("%02x", POLY1305_TAG(&ctx, i));
	printf("\n");

	return 0;
}
