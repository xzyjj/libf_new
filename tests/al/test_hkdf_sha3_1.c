#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/hmac_sha3.h>
#include <libf/al/hkdf_sha3.h>


int main(void) {
	uint8 buf[SHA3_512_LEN * 256];
	char *s = "123456";
	uint32 len = XSYMBOL(strlen)(s);

	HMAC_SHA3_NEW(ctx1);
	FSYMBOL(hmac_sha3_init)(&ctx1, (uint8 *)s, len, SHA3_512_TYPE);
	FSYMBOL(hmac_sha3_process)(&ctx1, (uint8 *)s, len);
	FSYMBOL(hmac_sha3_finish)(&ctx1);
	FSYMBOL(hkdf_sha3)(&(HMAC_SHA3_STATE(&ctx1, 0)), SHA3_512_TYPE,
		(uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	HMAC_SHA3_NEW(ctx2);
	FSYMBOL(hmac_sha3_init)(&ctx2, (uint8 *)s, len, SHA3_384_TYPE);
	FSYMBOL(hmac_sha3_process)(&ctx2, (uint8 *)s, len);
	FSYMBOL(hmac_sha3_finish)(&ctx2);
	FSYMBOL(hkdf_sha3)(&(HMAC_SHA3_STATE(&ctx2, 0)), SHA3_384_TYPE,
		(uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	HMAC_SHA3_NEW(ctx3);
	FSYMBOL(hmac_sha3_init)(&ctx3, (uint8 *)s, len, SHA3_256_TYPE);
	FSYMBOL(hmac_sha3_process)(&ctx3, (uint8 *)s, len);
	FSYMBOL(hmac_sha3_finish)(&ctx3);
	FSYMBOL(hkdf_sha3)(&(HMAC_SHA3_STATE(&ctx3, 0)), SHA3_256_TYPE,
		(uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	HMAC_SHA3_NEW(ctx4);
	FSYMBOL(hmac_sha3_init)(&ctx4, (uint8 *)s, len, SHA3_224_TYPE);
	FSYMBOL(hmac_sha3_process)(&ctx4, (uint8 *)s, len);
	FSYMBOL(hmac_sha3_finish)(&ctx4);
	FSYMBOL(hkdf_sha3)(&(HMAC_SHA3_STATE(&ctx4, 0)), SHA3_224_TYPE,
		(uint8 *)s, len,
		buf, 35);

	for (int32 i = 0; i < 35; i++)
		printf("%02x", buf[i]);
	printf("\n");

	return 0;
}
