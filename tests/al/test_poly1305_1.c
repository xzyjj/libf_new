#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/poly1305.h>


int main(void) {
	uint8 key[POLY1305_KEYLEN] = {
		0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33,
		0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8,
		0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd,
		0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b
		};
	char *str = "Cryptographic Forum Research Group";

	POLY1305_NEW(ctx);
	FSYMBOL(poly1305_init)(&ctx, key);

/*	FSYMBOL(poly1305_process)(&ctx, (uint8 *)str, XSYMBOL(strlen)(str));
	FSYMBOL(poly1305_finish)(&ctx);
*/
	FSYMBOL(poly1305)(&ctx, (uint8 *)str, XSYMBOL(strlen)(str));

	for (int32 i = 0; i < POLY1305_TAGLEN; i++)
		printf("%02x", POLY1305_TAG(&ctx, i));
	printf("\n");

	return 0;
}
