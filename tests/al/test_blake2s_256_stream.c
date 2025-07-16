#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/blake2s.h>


int main(void) {
	uint64 read_size = 0;
	BLAKE2S_NEW(ctx);

	FSYMBOL(blake2s_init)(&ctx, BLAKE2S_256_LEN);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(blake2s_process)(&ctx, (uint8 *)buf, read_size);
	}

	FSYMBOL(blake2s_finish)(&ctx);
	for (int32 i = 0; i < BLAKE2S_256_LEN; i++)
		printf("%02x", BLAKE2S_STATE(&ctx, i));
	printf("\n");

	return 0;
}
