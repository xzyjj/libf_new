#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_blake2s.h>
#include <libf/al/hkdf_blake2s.h>


int main(void) {
	uint8 buf[BLAKE2S_256_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_BLAKE2S_NEW(ctx);
	FSYMBOL(hmac_blake2s_init)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_blake2s_process)(&ctx, (uint8 *)s, len);
	FSYMBOL(hmac_blake2s_finish)(&ctx);
	FSYMBOL(hkdf_blake2s)(&(HMAC_BLAKE2S_STATE(&ctx, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
