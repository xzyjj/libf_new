#include <stdio.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/des.h>


static int32 _out(const uint8 *buf, void *arg) {
	fwrite(buf, 1, DES_BLOCKSIZE, stdout);

	return 0;
}

int main(void) {
	uint64 read_size = 0;
	DES_NEW(ctx);
	uint8 key[DES_KEYLEN];
	XSYMBOL(memset)(key, 0, DES_KEYLEN);
	key[0] = 0x12;
	key[1] = 0x23;
	key[2] = 0x45;
	FSYMBOL(des_init)(&ctx, key);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(des_encrypt_process)(&ctx, _out, NULL,
			(uint8 *)buf, read_size);
	}
	FSYMBOL(des_encrypt_finish)(&ctx, _out, NULL);

	return 0;
}
