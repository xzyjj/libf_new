#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha3.h>


int main(void) {
	uint64 read_size = 0;
	SHA3_NEW(ctx1);

	FSYMBOL(sha3_init)(&ctx1, SHA3_512_TYPE);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(sha3_process)(&ctx1, (uint8 *)buf, read_size);
	}

	FSYMBOL(sha3_finish)(&ctx1);
	for (int32 i = 0; i < SHA3_512_LEN; i++)
		printf("%02x", SHA3_STATE(&ctx1, i));
	printf("\n");

	return 0;
}
