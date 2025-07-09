#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/hmac_sha3.h>


int main(void) {
	uint64 read_size = 0;
	HMAC_SHA3_NEW(ctx_1);
	HMAC_SHA3_NEW(ctx_2);
	HMAC_SHA3_NEW(ctx_3);
	HMAC_SHA3_NEW(ctx_4);

	FSYMBOL(hmac_sha3_init)(&ctx_1, (const uint8 *)"123456", 6,
		SHA3_512_TYPE);
	FSYMBOL(hmac_sha3_init)(&ctx_2, (const uint8 *)"123456", 6,
		SHA3_384_TYPE);
	FSYMBOL(hmac_sha3_init)(&ctx_3, (const uint8 *)"123456", 6,
		SHA3_256_TYPE);
	FSYMBOL(hmac_sha3_init)(&ctx_4, (const uint8 *)"123456", 6,
		SHA3_224_TYPE);

	char buf[1 << 20];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(hmac_sha3_process)(&ctx_1, (uint8 *)buf, read_size);
		FSYMBOL(hmac_sha3_process)(&ctx_2, (uint8 *)buf, read_size);
		FSYMBOL(hmac_sha3_process)(&ctx_3, (uint8 *)buf, read_size);
		FSYMBOL(hmac_sha3_process)(&ctx_4, (uint8 *)buf, read_size);
	}

	FSYMBOL(hmac_sha3_finish)(&ctx_1);
	for (int32 i = 0; i < SHA3_512_LEN; i++)
		printf("%02x", HMAC_SHA3_STATE(&ctx_1, i));
	printf("\n");

	FSYMBOL(hmac_sha3_finish)(&ctx_2);
	for (int32 i = 0; i < SHA3_384_LEN; i++)
		printf("%02x", HMAC_SHA3_STATE(&ctx_2, i));
	printf("\n");

	FSYMBOL(hmac_sha3_finish)(&ctx_3);
	for (int32 i = 0; i < SHA3_256_LEN; i++)
		printf("%02x", HMAC_SHA3_STATE(&ctx_3, i));
	printf("\n");

	FSYMBOL(hmac_sha3_finish)(&ctx_4);
	for (int32 i = 0; i < SHA3_224_LEN; i++)
		printf("%02x", HMAC_SHA3_STATE(&ctx_4, i));
	printf("\n");

	return 0;
}
