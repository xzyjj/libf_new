#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/xxhash64.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	XXHASH64_NEW(ctx);
	FSYMBOL(xxhash64_init)(&ctx, 0);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(xxhash64_process)(&ctx, (uint8 *)buf, read_size);
		total_size += read_size;
	}

	uint64L hash = FSYMBOL(xxhash64_finish)(&ctx, total_size);
	for (int32 i = 0; i < 8; i++)
		printf("%02x", ((uint8 *)&hash)[i]);
	printf(" (0x%016llx)\n", hash);

	return 0;
}
