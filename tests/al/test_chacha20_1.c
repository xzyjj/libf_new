#include <stdio.h>
#include <time.h>
#include <libf/sl/xstdint.h>
#include <libf/sl/xstring.h>
#include <libf/al/chacha20.h>


int main(void) {
	clock_t start, end;
	double time;

	uint8 key[CHACHA20_KEYLEN], ran[CHACHA20_RANLEN];
	XSYMBOL(memset)(key, 12, CHACHA20_KEYLEN);
	XSYMBOL(memset)(ran, 67, CHACHA20_RANLEN);

	uint8 keystream[CHACHA20_BLOCKSIZE];
	uint32 state[16];
	char buf[CHACHA20_BLOCKSIZE];
	XSYMBOL(memset)(buf, 49, CHACHA20_BLOCKSIZE);

	uint32 n = 0;
	FSYMBOL(chacha20_init_state)(state, key, ran, n);

	start = clock();
	for (; n < 806596; n++) {
		FSYMBOL(chacha20_block)(state, (uint32 *)keystream);
		for (int32 i = 0; i < CHACHA20_BLOCKSIZE; i++)
			buf[i] ^= keystream[i];

/*		FSYMBOL(chacha20_block)(state, (uint32 *)keystream);
		for (int32 i = 0; i < CHACHA20_BLOCKSIZE; i++)
			buf[i] ^= keystream[i];

		fwrite(buf, 1, CHACHA20_KEYLEN, stdout);
*/

		state[CHACHA20_CINDEX]++;
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%.6fs (%.2fMiB/s)\n", time,
		((double)(CHACHA20_BLOCKSIZE * n) / time) / 1024 / 1024);

	return 0;
}
