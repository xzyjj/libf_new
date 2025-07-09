#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha256.h>
#include <libf/al/hkdf_sha256.h>


int main(void) {
	uint8 buf[SHA256_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_SHA256_NEW(ctx1);
	FSYMBOL(hmac_sha256_init)(&ctx1, (uint8 *)s, len);
	FSYMBOL(hmac_sha256_process)(&ctx1, (uint8 *)s, len);
	FSYMBOL(hmac_sha256_finish)(&ctx1, len);
	FSYMBOL(hkdf_sha256)(&(HMAC_SHA256_STATE(&ctx1, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	HMAC_SHA224_NEW(ctx2);
	FSYMBOL(hmac_sha224_init)(&ctx2, (uint8 *)s, len);
	FSYMBOL(hmac_sha224_process)(&ctx2, (uint8 *)s, len);
	FSYMBOL(hmac_sha224_finish)(&ctx2, len);
	FSYMBOL(hkdf_sha224)(&(HMAC_SHA224_STATE(&ctx2, 0)), (uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
