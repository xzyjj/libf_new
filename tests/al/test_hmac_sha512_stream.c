#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/hmac_sha512.h>


int main(void) {
	uint64 read_size = 0;
	HMAC_SHA512_NEW(ctx_1);
	HMAC_SHA384_NEW(ctx_2);
	FSYMBOL(hmac_sha512_init)(&ctx_1, (const uint8 *)"123456", 6);
	FSYMBOL(hmac_sha384_init)(&ctx_2, (const uint8 *)"123456", 6);

	char buf[1 << 20];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(hmac_sha512)(&ctx_1, (uint8 *)buf, read_size);
		FSYMBOL(hmac_sha384)(&ctx_2, (uint8 *)buf, read_size);
		break;
	}

	for (int32 i = 0; i < HAMC_SHA512_LEN; i++)
		printf("%02x", HMAC_SHA512_STATE(&ctx_1, i));
	printf("\n");

	for (int32 i = 0; i < HAMC_SHA384_LEN; i++)
		printf("%02x", HMAC_SHA384_STATE(&ctx_2, i));
	printf("\n");

	return 0;
}
