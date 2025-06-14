#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/md5.h>


int main(void) {
	uint64 read_size = 0, total_size = 0;
	MD5_NEW(ctx);
	FSYMBOL(md5_init)(&ctx);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(md5_process)(&ctx, (uint8 *)buf, read_size);
		total_size += read_size;
	}
	FSYMBOL(md5_finish)(&ctx, total_size);

	printf("\n");
	for (int32 i = 0; i < MD5_LEN; i++)
		printf("%02x", MD5_STATE(&ctx, i));
	printf("\n");

	return 0;
}
