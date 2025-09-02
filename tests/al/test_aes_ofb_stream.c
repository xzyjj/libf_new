#include <stdio.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/aes.h>


int main(void) {
	uint64 read_size = 0;
	struct aes_ctx aes_ctx;
	uint8 key[AES_128_KEYLEN];
	uint8 iv[AES_BLOCKSIZE];
	XSYMBOL(memset)(key, 0x3f, AES_128_KEYLEN);
	XSYMBOL(memset)(iv, 0x33, AES_BLOCKSIZE);
	FSYMBOL(aes_init)(&aes_ctx, key, AES_128_TYPE);

	char buf[1 << 17];
	while ((read_size = fread(buf, 1, sizeof(buf), stdin)) > 0) {
		FSYMBOL(aes_crypto_ofb)(&aes_ctx, iv, (uint8 *)buf, read_size);
		fwrite(buf, 1, read_size, stdout);
	}

	return 0;
}
