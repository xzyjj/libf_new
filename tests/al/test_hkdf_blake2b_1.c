#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_blake2b.h>
#include <libf/al/hkdf_blake2b.h>


int main(void) {
	uint8 buf[BLAKE2B_512_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_BLAKE2B_NEW(ctx);
	FSYMBOL(hmac_blake2b_init)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_blake2b_process)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_blake2b_finish)(&ctx);
	FSYMBOL(hkdf_blake2b)(&(HMAC_BLAKE2B_STATE(&ctx, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
