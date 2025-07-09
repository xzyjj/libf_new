#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha512.h>
#include <libf/al/hkdf_sha512.h>


int main(void) {
	uint8 buf[SHA512_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_SHA512_NEW(ctx1);
	FSYMBOL(hmac_sha512_init)(&ctx1, (uint8 *)s, len);
	FSYMBOL(hmac_sha512_process)(&ctx1, (uint8 *)s, len);
	FSYMBOL(hmac_sha512_finish)(&ctx1, len);
	FSYMBOL(hkdf_sha512)(&(HMAC_SHA512_STATE(&ctx1, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	HMAC_SHA384_NEW(ctx2);
	FSYMBOL(hmac_sha384_init)(&ctx2, (uint8 *)s, len);
	FSYMBOL(hmac_sha384_process)(&ctx2, (uint8 *)s, len);
	FSYMBOL(hmac_sha384_finish)(&ctx2, len);
	FSYMBOL(hkdf_sha384)(&(HMAC_SHA384_STATE(&ctx2, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
