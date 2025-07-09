#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/hmac_blake2s.h>


int main(void) {
	uint64 read_size = 0;
	HMAC_BLAKE2S_NEW(ctx);
	FSYMBOL(hmac_blake2s_init)(&ctx, (const uint8 *)"123456", 6);

	char buf[1 << 20];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(hmac_blake2s_process)(&ctx, (uint8 *)buf, read_size);
	}

	FSYMBOL(hmac_blake2s_finish)(&ctx);
	for (int32 i = 0; i < BLAKE2S_256_LEN; i++)
		printf("%02x", HMAC_BLAKE2S_STATE(&ctx, i));
	printf("\n");

	return 0;
}
