#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/blake2b.h>


int main(void) {
	uint64 read_size = 0;
	BLAKE2B_NEW(ctx1);
	BLAKE2B_NEW(ctx2);
	BLAKE2B_NEW(ctx3);
	BLAKE2B_NEW(ctx4);
	BLAKE2B_NEW(ctx5);

	FSYMBOL(blake2b_init)(&ctx1, BLAKE2B_512_LEN);
	FSYMBOL(blake2b_init)(&ctx2, BLAKE2B_384_LEN);
	FSYMBOL(blake2b_init)(&ctx3, BLAKE2B_256_LEN);
	FSYMBOL(blake2b_init)(&ctx4, BLAKE2B_224_LEN);
	FSYMBOL(blake2b_init)(&ctx5, 40);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(blake2b_process)(&ctx1, (uint8 *)buf, read_size);
		FSYMBOL(blake2b_process)(&ctx2, (uint8 *)buf, read_size);
		FSYMBOL(blake2b_process)(&ctx3, (uint8 *)buf, read_size);
		FSYMBOL(blake2b_process)(&ctx4, (uint8 *)buf, read_size);
		FSYMBOL(blake2b_process)(&ctx5, (uint8 *)buf, read_size);
	}

	FSYMBOL(blake2b_finish)(&ctx1);
	for (int32 i = 0; i < BLAKE2B_512_LEN; i++)
		printf("%02x", BLAKE2B_STATE(&ctx1, i));
	printf("\n");

	FSYMBOL(blake2b_finish)(&ctx2);
	for (int32 i = 0; i < BLAKE2B_384_LEN; i++)
		printf("%02x", BLAKE2B_STATE(&ctx2, i));
	printf("\n");

	FSYMBOL(blake2b_finish)(&ctx3);
	for (int32 i = 0; i < BLAKE2B_256_LEN; i++)
		printf("%02x", BLAKE2B_STATE(&ctx3, i));
	printf("\n");

	FSYMBOL(blake2b_finish)(&ctx4);
	for (int32 i = 0; i < BLAKE2B_224_LEN; i++)
		printf("%02x", BLAKE2B_STATE(&ctx4, i));
	printf("\n");

	FSYMBOL(blake2b_finish)(&ctx5);
	for (int32 i = 0; i < 40; i++)
		printf("%02x", BLAKE2B_STATE(&ctx5, i));
	printf("\n");

	return 0;
}
