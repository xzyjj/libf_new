#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha3.h>


int main(void) {
	uint64 read_size = 0;
	SHA3_NEW(ctx1);
	SHA3_NEW(ctx2);
	SHA3_NEW(ctx3);
	SHA3_NEW(ctx4);
	SHA3_NEW(ctx5);
	SHA3_NEW(ctx6);

	FSYMBOL(sha3_init)(&ctx1, SHA3_512_TYPE, 0);
	FSYMBOL(sha3_init)(&ctx2, SHA3_384_TYPE, 0);
	FSYMBOL(sha3_init)(&ctx3, SHA3_256_TYPE, 0);
	FSYMBOL(sha3_init)(&ctx4, SHA3_224_TYPE, 0);
	FSYMBOL(sha3_init)(&ctx5, SHA3_SHAKE128_TYPE, SHA3_256_LEN);
	FSYMBOL(sha3_init)(&ctx6, SHA3_SHAKE256_TYPE, SHA3_512_LEN);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(sha3_process)(&ctx1, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx2, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx3, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx4, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx5, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx6, (uint8 *)buf, read_size);
	}

	FSYMBOL(sha3_finish)(&ctx1);
	for (int32 i = 0; i < SHA3_512_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx1, i));
	printf("\n");

	FSYMBOL(sha3_finish)(&ctx2);
	for (int32 i = 0; i < SHA3_384_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx2, i));
	printf("\n");

	FSYMBOL(sha3_finish)(&ctx3);
	for (int32 i = 0; i < SHA3_256_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx3, i));
	printf("\n");

	FSYMBOL(sha3_finish)(&ctx4);
	for (int32 i = 0; i < SHA3_224_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx4, i));
	printf("\n");

	FSYMBOL(sha3_finish)(&ctx5);
	for (uint32 i = 0; i < SHA3_256_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx5, i));
	printf("\n");

	FSYMBOL(sha3_finish)(&ctx6);
	for (uint32 i = 0; i < SHA3_512_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx6, i));
	printf("\n");

	return 0;
}
