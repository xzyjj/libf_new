#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/aes.h>


static int32 _out(const uint8 *buf, void *arg) {
	fwrite(buf, 1, AES_BLOCKSIZE, stdout);

	return 0;
}

int main(void) {
	uint64 read_size = 0;
	AES_NEW(ctx);
	uint8 key[AES_128_KEYLEN];
	XSYMBOL(memset)(key, 0, AES_128_KEYLEN);
	FSYMBOL(aes_init)(&ctx, key, AES_128_TYPE);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(aes_decrypt_process)(&ctx, _out, NULL,
			(uint8 *)buf, read_size);
	}

	return 0;
}
