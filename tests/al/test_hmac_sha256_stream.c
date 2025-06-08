#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/hmac_sha256.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	HMAC_SHA256_NEW(ctx_1);
	HMAC_SHA224_NEW(ctx_2);
	FSYMBOL(hmac_sha256_init)(&ctx_1, (const uint8 *)"123456", 6);
	FSYMBOL(hmac_sha224_init)(&ctx_2, (const uint8 *)"123456", 6);

	char buf[1 << 20];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(hmac_sha256_process)(&ctx_1, (uint8 *)buf, read_size);
		FSYMBOL(hmac_sha224_process)(&ctx_2, (uint8 *)buf, read_size);
		total_size += read_size;
	}

	FSYMBOL(hmac_sha256_finish)(&ctx_1, total_size);
	for (int32 i = 0; i < SHA256_LEN; i++)
		printf("%02x", HMAC_SHA256_STATE(&ctx_1, i));
	printf("\n");

	FSYMBOL(hmac_sha224_finish)(&ctx_2, total_size);
	for (int32 i = 0; i < SHA224_LEN; i++)
		printf("%02x", HMAC_SHA224_STATE(&ctx_2, i));
	printf("\n");

	return 0;
}
