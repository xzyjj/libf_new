#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/hmac_sha1.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	HMAC_SHA1_NEW(ctx);
	FSYMBOL(hmac_sha1_init)(&ctx, (const uint8 *)"123456", 6);

	char buf[1 << 20];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(hmac_sha1_process)(&ctx, (uint8 *)buf, read_size);
		total_size += read_size;
	}

	FSYMBOL(hmac_sha1_finish)(&ctx, total_size);
	for (int32 i = 0; i < SHA1_LEN; i++)
		printf("%02x", HMAC_SHA1_STATE(&ctx, i));
	printf("\n");

	return 0;
}
