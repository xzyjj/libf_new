#include <stdio.h>
#include <time.h>
#include <libf/sl/xstddef.h>
#include <libf/sl/xstring.h>
#include <libf/al/aes.h>


int main(void) {
	clock_t start, end;
	double time;

	uint8 key[AES_128_KEYLEN];
	XSYMBOL(memset)(key, 0, AES_128_KEYLEN);

	AES_NEW(ctx);
	FSYMBOL(aes_init)(&ctx, key, AES_128_TYPE);

	uint8 state[AES_BLOCKSIZE], iv[AES_BLOCKSIZE];
	XSYMBOL(memset)(state, 0, AES_BLOCKSIZE);
	XSYMBOL(memset)(iv, 0, AES_BLOCKSIZE);

	start = clock();
	int32 n = 0;
	for (; n < 806596; n++) {
		FSYMBOL(aes_crypto_ofb)(&ctx, iv, state, AES_BLOCKSIZE);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("en: %.6fs (%.2fMiB/s)\n", time, ((double)(16 * n) / time) / 1024 / 1024);

	start = clock();
	n = 0;
	for (; n < 806596; n++) {
		FSYMBOL(aes_crypto_ofb)(&ctx, iv, state, AES_BLOCKSIZE);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("de: %.6fs (%.2fMiB/s)\n", time, ((double)(16 * n) / time) / 1024 / 1024);

	return 0;
}
