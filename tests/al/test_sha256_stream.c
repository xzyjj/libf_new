#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha256.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	SHA256_NEW(ctx1);
	SHA224_NEW(ctx2);
	FSYMBOL(sha256_init)(&ctx1);
	FSYMBOL(sha224_init)(&ctx2);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(sha256_process)(&ctx1, (uint8 *)buf, read_size);
		FSYMBOL(sha256_process)(&ctx2, (uint8 *)buf, read_size);
		total_size += read_size;
	}

	FSYMBOL(sha256_finish)(&ctx1, total_size);
	for (int32 i = 0; i < SHA256_LEN; i++)
		printf("%02x", SHA256_STATE(&ctx1, i));
	printf("\n");

	FSYMBOL(sha256_finish)(&ctx2, total_size);
	for (int32 i = 0; i < SHA224_LEN; i++)
		printf("%02x", SHA224_STATE(&ctx2, i));
	printf("\n");

	return 0;
}
