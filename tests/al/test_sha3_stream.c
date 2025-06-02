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

	FSYMBOL(sha3_init)(&ctx1, SHA3_512_TYPE);
	FSYMBOL(sha3_init)(&ctx2, SHA3_384_TYPE);
	FSYMBOL(sha3_init)(&ctx3, SHA3_256_TYPE);
	FSYMBOL(sha3_init)(&ctx4, SHA3_224_TYPE);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(sha3_process)(&ctx1, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx2, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx3, (uint8 *)buf, read_size);
		FSYMBOL(sha3_process)(&ctx4, (uint8 *)buf, read_size);
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

	return 0;
}
