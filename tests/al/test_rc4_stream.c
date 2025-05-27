#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/al/rc4.h>


int main(void) {
	uint64 read_size = 0;
	RC4_NEW(ctx);
	FSYMBOL(rc4_init)(&ctx, (uint8 *)"1234", 4);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(rc4_crypto)(&ctx, (uint8 *)buf, read_size);
		fwrite(buf, 1, read_size, stdout);
	}

	return 0;
}
