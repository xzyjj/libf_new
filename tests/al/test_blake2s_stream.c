#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/blake2s.h>


int main(void) {
	uint64 read_size = 0;
	BLAKE2S_NEW(ctx1);
	BLAKE2S_NEW(ctx2);
	BLAKE2S_NEW(ctx3);
	BLAKE2S_NEW(ctx4);

	FSYMBOL(blake2s_init)(&ctx1, BLAKE2S_256_LEN);
	FSYMBOL(blake2s_init)(&ctx2, BLAKE2S_196_LEN);
	FSYMBOL(blake2s_init)(&ctx3, BLAKE2S_128_LEN);
	FSYMBOL(blake2s_init)(&ctx4, 20);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(blake2s_process)(&ctx1, (uint8 *)buf, read_size);
		FSYMBOL(blake2s_process)(&ctx2, (uint8 *)buf, read_size);
		FSYMBOL(blake2s_process)(&ctx3, (uint8 *)buf, read_size);
		FSYMBOL(blake2s_process)(&ctx4, (uint8 *)buf, read_size);
	}

	FSYMBOL(blake2s_finish)(&ctx1);
	for (int32 i = 0; i < BLAKE2S_256_LEN; i++)
		printf("%02x", BLAKE2S_STATE(&ctx1, i));
	printf("\n");

	FSYMBOL(blake2s_finish)(&ctx2);
	for (int32 i = 0; i < BLAKE2S_196_LEN; i++)
		printf("%02x", BLAKE2S_STATE(&ctx2, i));
	printf("\n");

	FSYMBOL(blake2s_finish)(&ctx3);
	for (int32 i = 0; i < BLAKE2S_128_LEN; i++)
		printf("%02x", BLAKE2S_STATE(&ctx3, i));
	printf("\n");

	FSYMBOL(blake2s_finish)(&ctx4);
	for (int32 i = 0; i < 20; i++)
		printf("%02x", BLAKE2S_STATE(&ctx4, i));
	printf("\n");

	return 0;
}
