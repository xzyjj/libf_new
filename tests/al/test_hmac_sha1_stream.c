#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/hmac_sha1.h>


int main(void) {
	uint64 read_size = 0;
	HMAC_SHA1_NEW(ctx);
	FSYMBOL(hmac_sha1_init)(&ctx, (const uint8 *)"123456", 6);

	char buf[1 << 20];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(hmac_sha1)(&ctx, (uint8 *)buf, read_size);
		break;
	}

	for (int32 i = 0; i < HAMC_SHA1_LEN; i++)
		printf("%02x", HMAC_SHA1_STATE(&ctx, i));
	printf("\n");

	return 0;
}
