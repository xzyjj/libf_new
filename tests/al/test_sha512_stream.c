#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/al/sha512.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	SHA512_NEW(ctx1);
	SHA384_NEW(ctx2);
	FSYMBOL(sha512_init)(&ctx1);
	FSYMBOL(sha384_init)(&ctx2);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(sha512_process)(&ctx1, (uint8 *)buf, read_size);
		FSYMBOL(sha512_process)(&ctx2, (uint8 *)buf, read_size);
		total_size += read_size;
	}

	FSYMBOL(sha512_finish)(&ctx1, total_size);
	for (int32 i = 0; i < SHA512_LEN; i++)
		printf("%02x", SHA512_STATE(&ctx1, i));
	printf("\n");

	FSYMBOL(sha512_finish)(&ctx2, total_size);
	for (int32 i = 0; i < SHA384_LEN; i++)
		printf("%02x", SHA384_STATE(&ctx2, i));
	printf("\n");

	return 0;
}
