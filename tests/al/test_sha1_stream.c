#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha1.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	SHA1_NEW(ctx);
	FSYMBOL(sha1_init)(&ctx);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(sha1_process)(&ctx, (uint8 *)buf, read_size);
		total_size += read_size;
	}
	FSYMBOL(sha1_finish)(&ctx, total_size);

	printf("\n");
	for (int32 i = 0; i < SHA1_LEN; i++)
		printf("%02x", SHA1_STATE(&ctx, i));
	printf("\n");

	return 0;
}
